#include "driver/fs.h"

fs_index_t *fs_index = NULL;    // Look-up table for objectid and filename
uint16_t fs_index_size = 0;

void ICACHE_FLASH_ATTR fs_init()
{
    uint16_t *object_ids;           // tmp buffer for index page
    uint8_t block;
    for (block = 0; block < (FS_WHOLE_SIZE / FS_BLOCK_SIZE); block++)
    {
        uint8_t numberOfnewID = fs_load_index_page(block, &object_ids);       //Here we store the number of new ObjIDs that we found on the current block index page
        uint16_t index;
#if FS_DEBUG
        for (index = 0; index < numberOfnewID; index++)
        {
            os_printf("\nOBJ ID : %d -> 0x%04x", index, object_ids[index]);
        }
#endif
        for (index = 0; index < numberOfnewID; index++)
        {
            //get only meta pages
            if (object_ids[index] & 0x8000)
            {
                fs_index_size++;
                if(fs_index){
                    fs_index = (fs_index_t *)os_realloc(fs_index, sizeof(fs_index_t) * fs_index_size);
                } else {
                    fs_index = (fs_index_t *)os_malloc(sizeof(fs_index_t) * fs_index_size);
                }
                uint32_t buffer_objid;
                spi_flash_read(FS_BASE_ADDRESS + (block * FS_BLOCK_SIZE) + ((uint16_t)FS_PAGE_SIZE * (index + 1)), &buffer_objid, 4);
#if FS_DEBUG
                os_printf("\nOBJ_ID: %x", buffer_objid);
#endif
                fs_index[fs_index_size - 1].object_id = (uint16_t)(buffer_objid & 0xFFFF);
                uint32_t buffer_filename[FS_META_MAX_FILENAME_LEN / 4];
                spi_flash_read(FS_BASE_ADDRESS + (block * FS_BLOCK_SIZE) + ((uint16_t)FS_PAGE_SIZE * (index + 1)) + FS_META_FILENAME_OFFSET, buffer_filename, FS_META_MAX_FILENAME_LEN);
                uint8_t i;
                for (i = 0; i < FS_META_MAX_FILENAME_LEN / 4; i++)
                {
#if FS_DEBUG
                    os_printf("\nFilename: %x", buffer_filename[i]);
#endif
                    fs_index[fs_index_size - 1].filename[i * 4] = LOWER_8(buffer_filename[i]);
                    fs_index[fs_index_size - 1].filename[i * 4 + 1] = LOWER_MID_8(buffer_filename[i]);
                    fs_index[fs_index_size - 1].filename[i * 4 + 2] = UPPER_MID_8(buffer_filename[i]);
                    fs_index[fs_index_size - 1].filename[i * 4 + 3] = UPPER_8(buffer_filename[i]);
                }
#if FS_DEBUG
                os_printf("\nDecoded Filename: %s", fs_index[fs_index_size - 1].filename);
#endif
                fs_index[fs_index_size - 1].block = block;
                fs_index[fs_index_size - 1].page = index + 1;                               // PAGE0 is allway the index page so we count from 1 not 0
#if FS_DEBUG
                os_printf("\nCurernt block: %d", fs_index[fs_index_size - 1].block);
                os_printf("\nCurernt page: %d", fs_index[fs_index_size - 1].page);
#endif
            }
        }
        os_free(object_ids);
    }
}

uint8_t ICACHE_FLASH_ATTR fs_load_index_page(uint8_t indexpage_offset, uint16_t **object_ids)
{
    uint32_t obj_id[FS_PAGE_SIZE / 4];
    spi_flash_read((FS_BASE_ADDRESS + indexpage_offset * (uint16_t)FS_BLOCK_SIZE), obj_id, (uint16_t)FS_PAGE_SIZE);
    uint16_t index;
#if FS_DEBUG
    for (index = 0; index < FS_PAGE_SIZE / 16; index++)
    {
        os_printf("\nOffset %d -> %08x,%08x,%08x,%08x", index*16, obj_id[index * 4 + 0], obj_id[index * 4 + 1], obj_id[index * 4 + 2], obj_id[index * 4 + 3]);
    }
#endif
    uint16_t size = 0;
    while (obj_id[size] != 0xFFFFFFFF && size < FS_PAGE_SIZE / 4)
    {
        size++;
    }
    *object_ids = (uint16_t *)os_malloc(sizeof(uint16_t) * size * 2);
    for (index = 0; index < size ; index++)
    {
        *(*object_ids + index * 2) = LOWER_16(obj_id[index]);
        *(*object_ids + index * 2 + 1) = UPPER_16(obj_id[index]);
    }
    size *= 2;
    index--;
    #ifdef FS_DEBUG
    os_printf("\nLastcheck : Lower: %04x, Upper: %04x", LOWER_16(obj_id[index]), UPPER_16(obj_id[index]));
    #endif
    if( (LOWER_16(obj_id[index]) != 0xFFFF && UPPER_16(obj_id[index]) == 0xFFFF) | (LOWER_16(obj_id[index]) == 0xFFFF && UPPER_16(obj_id[index]) != 0xFFFF) )
        size--;
    return size;
}

uint16_t ICACHE_FLASH_ATTR fs_openfile(const char *file_name, fileobject_t *fn){
    uint16_t counter = 0;

    #ifdef FS_DEBUG
    while(counter < fs_index_size ){
        os_printf("\nFS_INDEX \tFilename: %s ID: %04x block: %d page: %d", fs_index[counter].filename, fs_index[counter].object_id, fs_index[counter].block, fs_index[counter].page);
        counter++;
    }
    counter = 0;
    #endif
    //Search for OBJ_ID in fs_index
    while( str_cmp(file_name, &fs_index[counter].filename[0]) && (counter < fs_index_size) ){
        counter++;
    }

    if(counter == fs_index_size){
        #ifdef FS_DEBUG
        os_printf("\nFile not found.");
        #endif
        return FS_ERR_FILE_NOT_FOUND_BY_NAME;
    }

    #ifdef FS_DEBUG
    os_printf("\nFilename: %s ID: %04x block: %d page: %d", fs_index[counter].filename, fs_index[counter].object_id, fs_index[counter].block, fs_index[counter].page);
    #endif
    //Updating the file handler
    fn->objid = fs_index[counter].object_id;
    fn->filepointer = 0;
    fn->block = fs_index[counter].block;
    // Get the size of the requested file
    uint32_t tmp;
    spi_flash_read(FS_BASE_ADDRESS + fn->block * FS_BLOCK_SIZE
                     + fs_index[counter].page * FS_PAGE_SIZE 
                     + FS_META_SIZE_OFFSET, &tmp, 4);
    fn->size = LOWER_16(tmp);

    #ifdef FS_DEBUG
    os_printf(" offset: %d buffer: %08x datasize: %d",fn->block * FS_BLOCK_SIZE + fs_index[counter].page * FS_PAGE_SIZE + FS_META_SIZE_OFFSET, tmp, fn->size);
    #endif

    // Calculate how many data page we have based on its size
    uint16_t numberofdatapages;
    uint32_t *buffer;
    if(fn->size % FS_DATA_DATAPERPAGE){
        numberofdatapages = fn->size / FS_DATA_DATAPERPAGE +1;
    } else{
        numberofdatapages = fn->size / FS_DATA_DATAPERPAGE;
    }

    #ifdef FS_DEBUG
    os_printf(" datapages: %d", numberofdatapages);
    #endif

    // Calculating temporary buffer size and read data page offsets from flash
    if(numberofdatapages % 4){
        buffer = (uint32_t *) os_malloc(sizeof(uint32_t) * numberofdatapages * 2 / 4 + 1); //shuld be multiply of 4 and must be muliplied by 2 because the datapages are 2bytes values 
        spi_flash_read(FS_BASE_ADDRESS + fn->block * FS_BLOCK_SIZE
                     + fs_index[counter].page * FS_PAGE_SIZE +
                     FS_META_DATAPAGES_OFFSET, buffer, ((numberofdatapages * 2 / 4 + 1) *  4));
    } else{
        buffer = (uint32_t *) os_malloc(sizeof(uint32_t) * numberofdatapages / 2);
        spi_flash_read(FS_BASE_ADDRESS + fn->block * FS_BLOCK_SIZE
                     + fs_index[counter].page * FS_PAGE_SIZE +
                     FS_META_DATAPAGES_OFFSET, buffer, numberofdatapages * 2 );
    }
    fn->pages = (uint16_t *) os_malloc(sizeof(uint16_t) * numberofdatapages);

    //Parsing the result and updating the file handler
    uint8_t i;
    for(i = 0; i < numberofdatapages; i++){
        if(i % 2){
            fn->pages[i] = UPPER_16(buffer[i / 2]);
            #ifdef  FS_DEBUG
            os_printf(" , %04x", fn->pages[i]);
            #endif
        } else{
            fn->pages[i] = LOWER_16(buffer[i / 2]);
            #ifdef  FS_DEBUG
            os_printf("\nDATAPAGES : %04x", fn->pages[i]);
            #endif
        }
    }
    os_free(buffer);
    return numberofdatapages;

}
uint8_t ICACHE_FLASH_ATTR fs_readfile_raw(fileobject_t *fn, uint32_t *buffer, uint16_t size_in_words){
    if( size_in_words * 4  > fn->size){                                                                     // if we requesting too much data we get what we have
        size_in_words = fn->size / 4 * 4;
    }

    uint16_t buffer_index = 0;
    while(size_in_words){

        uint8_t current_read_len = size_in_words * 4 / FS_DATA_DATAPERPAGE ?  FS_DATA_DATAPERPAGE : size_in_words * 4 % FS_DATA_DATAPERPAGE;     // how many bytes we will read
        uint8_t current_datapage_index  = fn->filepointer / FS_DATA_DATAPERPAGE;                                                                 // Datapage index for fn->pages[]
        uint8_t current_data_offset     = fn->filepointer % FS_DATA_DATAPERPAGE;                                                                 // offset from the start of data page 
        #ifdef FS_DEBUG
        os_printf("\nCurrent_read_len: %d, Dp_index: %d, Dp_offset: %04x, D_offset: %d, fpointer: %d, buffer_i: %d",
                     current_read_len, current_datapage_index, fn->pages[current_datapage_index], current_data_offset, fn->filepointer, buffer_index);
        #endif
        spi_flash_read(FS_BASE_ADDRESS + fn->pages[current_datapage_index] + FS_DATA_DATAOFFSET + current_data_offset, &buffer[buffer_index], current_read_len);
        fn->filepointer += current_read_len;
        buffer_index += current_read_len / 4;
        size_in_words -= current_read_len / 4;
        
        #ifdef FS_DEBUG
        uint16_t i;
        for(i = 0; i<buffer_index; i++){
            if(i % 4)
                os_printf("\t %d->%08x",i ,buffer[i]);
            else
                os_printf("\n\tBuffer: %d->%08x",i ,buffer[i]);
        }
        #endif
    }

}