#include "driver/fs.h"
fs_index_t *fs_index;            // Look-up table for objectids and filenames
uint16_t fs_index_size;
//uint16_t *preallocated_pages;     // List of offsets where data are writen or going to be write, but the index page not updated yet
//uint8_t preallocated_pages_size;
blk_cacke_t blk_cache;

void ICACHE_FLASH_ATTR fs_init(){
    fs_index = NULL;
    fs_index_size = 0;
    blk_cache.block_cache = NULL;
    blk_cache.modified_pages = NULL;
    blk_cache.modified_pages_size = 0;
    //preallocated_pages = NULL;
    //preallocated_pages_size = 0;
    uint16_t *object_ids;           // tmp buffer for index page
    uint8_t block;
    for (block = 0; block < (FS_WHOLE_SIZE / FS_BLOCK_SIZE); block++)
    {
        uint8_t numberOfnewID = _fs_load_index_page(block, &object_ids);       //Here we store the number of new ObjIDs that we found on the current block index page
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
                fs_index[fs_index_size - 1].flags = (1 << FSINDEX_FLAGS_FILE_IN_FLASH);
                #if FS_DEBUG
                os_printf("\nCurernt block: %d", fs_index[fs_index_size - 1].block);
                os_printf("\nCurernt page: %d", fs_index[fs_index_size - 1].page);
                #endif
            }
        }
        os_free(object_ids);
    }
}

int8_t ICACHE_FLASH_ATTR fs_openfile(const char *file_name, fileobject_t *fn){
    uint16_t counter = 0;

    #ifdef FS_DEBUG
    while(counter < fs_index_size ){
        os_printf("\nFS_INDEX \tFilename: %s ID: %04x block: %d page: %d flags 0x%02x", fs_index[counter].filename, fs_index[counter].object_id, fs_index[counter].block, fs_index[counter].page, fs_index[counter].flags);
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
    return 0;

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

int8_t ICACHE_FLASH_ATTR fs_createNewFile(fileobject_t *fn, char *file_name){
    // Are this file allready exist?
    uint16_t counter = 0;
    while(counter < fs_index_size){
        if(!str_cmp(file_name, &fs_index[counter].filename[0])){
            return FS_ERR_FILE_ALLREADY_EXIST;
            #ifdef FS_DEBUG
            os_printf("\n %s allready exist" ,file_name);
            #endif
        }
        counter++;
    }
    // Get a free object id for this file
    uint16_t OBJ_ID = 0x8001;
    counter = 0;
    while(counter < fs_index_size){
        if(fs_index[counter].object_id == OBJ_ID){
            OBJ_ID++;
            counter = 0;
        } else{
            counter++;
        }
    }
    // Check the name of the file
    uint8_t filename_len = 0;
    if(strlen(file_name) > FS_META_MAX_FILENAME_LEN){
        return FS_ERR_FILE_FILENAME_TOO_LONG;
    } else {
        uint8_t filename_len = strlen(file_name);
    }
    // Update fs_index and filehandler
    fs_index_size++;
    fs_index = (fs_index_t *)os_realloc(fs_index, sizeof(fs_index_t) * fs_index_size);
    strcpy(fs_index[fs_index_size - 1].filename, file_name );
    fs_index[fs_index_size - 1].object_id   = OBJ_ID;
    fs_index[fs_index_size - 1].block       = 0;
    fs_index[fs_index_size - 1].page        = 0;
    fs_index[fs_index_size - 1].flags       = (0 << FSINDEX_FLAGS_FILE_IN_FLASH);
    fn->objid       = OBJ_ID;
    fn->block       = 0;
    fn->cache       = NULL;
    fn->cache_len   = 0;
    fn->filepointer = 0;
    fn->size        = 0;
    fn->pages       = NULL;
    fn->pages_len   = 0;
    #ifdef FS_DEBUG
    os_printf("\nNew OBJID is %04x", fs_index[fs_index_size - 1].object_id);
    #endif    

    
}

//this is writing to the cache first
int8_t ICACHE_FLASH_ATTR fs_write(fileobject_t *fn, const char *data, uint16_t len){
    //Memory allocation for file cache
    if(fn->cache == NULL){
        #ifdef FS_DEBUG
        os_printf("\n\rAllocating memory for cache");
        #endif
        fn->cache = (char *)os_malloc(sizeof(fn->cache) * len);
        fn->cache_len = len;
        #ifdef FS_DEBUG
        os_printf("\n\rCache address is: 0x%x, cache size %d", fn->cache, fn->cache_len);
        #endif
    }else{
        #ifdef FS_DEBUG
        os_printf("\n\rReallocating memory for cache");
        #endif
        fn->cache = (char *)os_realloc(fn->cache, (fn->cache_len + len) * sizeof(fn->cache));
        fn->cache_len += len;
        #ifdef FS_DEBUG
        os_printf("\n\rCache address is: 0x%x, cache size %d", fn->cache, fn->cache_len);
        #endif
    }
    memcpy(fn->cache, data, len);
    if(fn->cache_len >= FS_BLOCK_SIZE - FS_PAGE_SIZE){          //Every block need an index page
        uint8_t freeblock = _fs_getfreeblock();
        //_fs_writeblock(fn, freeblock);
    }
    
}
uint8_t ICACHE_FLASH_ATTR fs_flush(fileobject_t *fn){
    char header[FS_DATA_HEADER_LEN];
    header[0] = fn->objid & 0xFF;
    header[1] = fn->objid >> 8;
    header[4] = 0xFC;

    //Calculating how many data page we need to store cached data
    uint8_t pages_len = 1;
    pages_len++;                                                                                    //One more for the METAPAGE
    if(fn->cache_len % FS_DATA_DATAPERPAGE){
        pages_len += (uint8_t)(fn->cache_len / FS_DATA_DATAPERPAGE);
    } else {
        pages_len += fn->cache_len / FS_DATA_DATAPERPAGE;
    }
    uint16_t freepage[pages_len];
    _fs_getfreepages(freepage, pages_len);
    uint8_t blkcache_start = blk_cache.modified_pages_size;                                         //This is storing the last element position
    //Allocating memory for preallocated pages table
    if(blk_cache.modified_pages == NULL){
        blk_cache.modified_pages = (uint16_t *) os_malloc(pages_len * sizeof(uint16_t));
        blk_cache.modified_pages_size = pages_len;
    } else {
        blk_cache.modified_pages = (uint16_t *) os_realloc(blk_cache.modified_pages, (pages_len + blk_cache.modified_pages_size) * sizeof(uint16_t));
        blk_cache.modified_pages_size += pages_len;
    }

    //Building data pages
    uint8_t page;
    for (page = 0; page < pages_len; page++){
        //Updating SNAP IX
        header[2] = page & 0xF0;
        header[3] = page >> 8;
        uint8_t transfered_data;
        //Transfering data to pagebuilder
        if(fn->cache_len / FS_DATA_DATAPERPAGE){
            transfered_data = FS_DATA_DATAPERPAGE;
            _fs_buildPage(freepage[page], header, FS_DATA_HEADER_LEN, fn->cache, transfered_data, 0);
            fn->cache += transfered_data;
            fn->cache_len -= transfered_data;
        } else {
            transfered_data =fn->cache_len % FS_DATA_DATAPERPAGE;
            _fs_buildPage(freepage[page], header, FS_DATA_HEADER_LEN, fn->cache, transfered_data, 0);
        }
        //Updating pages[] table
        if (fn->pages == NULL)
        {
            #ifdef FS_DEBUG
            os_printf("\n\rAllocating memory for pages");
            #endif
            fn->pages = (uint16_t *)os_malloc(sizeof(fn->pages));
            fn->pages_len++;
            #ifdef FS_DEBUG
            os_printf("\n\rPages address is: 0x%x, pages size: %d", fn->pages, fn->pages_len);
            #endif
        }
        else
        {
            #ifdef FS_DEBUG
            os_printf("\n\rReallocating memory for pages");
            #endif
            fn->pages_len++;
            fn->pages = (uint16_t *)os_realloc(fn->pages, fn->pages_len * sizeof(fn->pages));
            #ifdef FS_DEBUG
            os_printf("\n\rPages address is: 0x%x, pages size: %d", fn->pages, fn->pages_len);
            #endif
        }
        fn->pages[fn->pages_len - 1] = freepage[page];
        fn->size += transfered_data;

        //Updating modified pages table
        blk_cache.modified_pages[blkcache_start + page] = freepage[page];
    }
    uint32_t *meta_page;
    uint8_t metapage_len = _fs_buildMetaPage(fn, *meta_page);
    _fs_getfreepages(freepage, 1);
    _fs_buildPage(freepage[0], meta_page, metapage_len, NULL, 0, 0);

    os_free(fn->cache);
    fn->cache = NULL;
    fn->cache_len = 0;
}
