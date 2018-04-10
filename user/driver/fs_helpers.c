#include "driver/fs.h"

uint8_t ICACHE_FLASH_ATTR _fs_load_index_page(uint8_t buffer_offset, uint16_t **object_ids){
    uint32_t obj_id[FS_PAGE_SIZE / 4];
    spi_flash_read((FS_BASE_ADDRESS + buffer_offset * (uint16_t)FS_BLOCK_SIZE), obj_id, (uint16_t)FS_PAGE_SIZE);
    uint16_t index;
    #if FS_DEBUG
    for (index = 0; index < FS_PAGE_SIZE / 16; index++){
        os_printf("\nOffset %d -> %08x,%08x,%08x,%08x", index * 16, obj_id[index * 4 + 0], obj_id[index * 4 + 1], obj_id[index * 4 + 2], obj_id[index * 4 + 3]);
    }
    #endif
    uint16_t size = 0;
    while (obj_id[size] != 0xFFFFFFFF && size < FS_PAGE_SIZE / 4){
        size++;
    }
    *object_ids = (uint16_t *)os_malloc(sizeof(uint16_t) * size * 2);
    for (index = 0; index < size; index++){
        *(*object_ids + index * 2) = LOWER_16(obj_id[index]);
        *(*object_ids + index * 2 + 1) = UPPER_16(obj_id[index]);
    }
    size *= 2;
    index--;
    #ifdef FS_DEBUG
    os_printf("\nLastcheck : Lower: %04x, Upper: %04x", LOWER_16(obj_id[index]), UPPER_16(obj_id[index]));
    #endif
    if ((LOWER_16(obj_id[index]) != 0xFFFF && UPPER_16(obj_id[index]) == 0xFFFF) | (LOWER_16(obj_id[index]) == 0xFFFF && UPPER_16(obj_id[index]) != 0xFFFF))
        size--;
    return size;
}

uint8_t ICACHE_FLASH_ATTR _fs_getfreepages(uint16_t *buffer, uint8_t numberOfFreePages){
    uint16_t freePageOffset = 0;
    uint8_t current_block = 0;
    uint8_t freepage_counter = 0;
    while (freepage_counter < numberOfFreePages){
        do{
            uint16_t *object_ids;
            uint8_t numberOfIDs = _fs_load_index_page(current_block, &object_ids);
            uint8_t counter = 0;
            while (counter < numberOfIDs){
                #ifdef FS_DEBUG
                os_printf("\nFree page finder on block %d OBJ_ID 0x%04x", current_block, object_ids[counter]);
                #endif
                if (object_ids[counter] == 0x0000 | object_ids[counter] == 0xFFFF){
                    freePageOffset = current_block * FS_BLOCK_SIZE + (counter + 1) * FS_PAGE_SIZE;
                    counter = numberOfIDs;
                }else{
                    counter++;
                }
            }
            current_block++;
        } while (!freePageOffset);
        buffer[freepage_counter] = freePageOffset;
        freepage_counter++;
    }
    #ifdef FS_DEBUG
    uint8_t i;
    os_printf("\nFree page offset:");
    for(i = 0; i < freepage_counter; i++){
        os_printf("0x%04x\t", buffer[i]);
    }
    #endif
}
void ICACHE_FLASH_ATTR _fs_dump_fsindex(){
    uint16_t counter = 0;
    os_printf("FS_INDEX_DUMP");
    while(counter < fs_index_size ){
        os_printf("Filename: %s OBJID: %04x block: %d page: %d flags 0x%02x", fs_index[counter].filename, fs_index[counter].object_id, fs_index[counter].block, fs_index[counter].page, fs_index[counter].flags);
        counter++;
    }
}
void ICACHE_FLASH_ATTR _fs_dump_fileobject(fileobject_t *fn){
    #ifdef FS_DEBUG
    os_printf("\n\rOBJID: %04x\tfilepointer: %d\tSize: %d\tBlock: %d\t", fn->objid, fn->filepointer, fn->size, fn->block);
    #endif
    // Calculate how many data page we have based on its size and print it out
    uint16_t numberofdatapages;
    if(fn->size % FS_DATA_DATAPERPAGE && fn->size != 0){
        numberofdatapages = fn->size / FS_DATA_DATAPERPAGE +1;
    } else{
        numberofdatapages = fn->size / FS_DATA_DATAPERPAGE;
    }
    #ifdef FS_DEBUG
    os_printf("Number of datapage: %d\n\rDatapage_index:\n\r", numberofdatapages);
    #endif
    uint16_t counter;
    for(counter = 0; counter < numberofdatapages; counter++){
        if(counter % 6)
            os_printf("%04x, ", fn->pages[counter]);
        else
            os_printf("\n%04x, ", fn->pages[counter]);
    }
}
uint8_t ICACHE_FLASH_ATTR _fs_getfreeblock(){
    #ifdef FS_DEBUG
    os_printf("\n\rFreeblock finder\n\r");
    #endif
    uint8_t current_block = 0;
    while(current_block < FS_WHOLE_SIZE / FS_BLOCK_SIZE){
        uint32_t buffer[FS_PAGE_SIZE / 4];
        spi_flash_read(FS_BASE_ADDRESS + current_block * FS_BLOCK_SIZE, buffer, FS_PAGE_SIZE);
        uint8_t counter = 0;
        #ifdef FS_DEBUG
        os_printf("Checking block %d", current_block);
        #endif
        while(counter < FS_PAGE_SIZE / 4 - 1){             //the last dword in the index page has different meaning
            #ifdef FS_DEBUG
            if(! (counter % 6) )
                os_printf("\n\r");
            os_printf("%02d->0x%08x,", counter, buffer[counter]);
            #endif
            if(buffer[counter] == 0x00000000 | buffer[counter] == 0xFFFFFFFF){
                counter++;
            } else {
                counter = 0xFF;                             // This means this is a used block
                #ifdef FS_DEBUG
                os_printf("\n\rThis block are used\n\r");
                #endif
            }
        }
        if(counter == (FS_PAGE_SIZE / 4 - 1)){
            return current_block;
        } else if( (current_block == FS_WHOLE_SIZE / FS_BLOCK_SIZE -1) && counter == 0xFF) {
             return current_block = 0xff;                           // No more free block on the filesystem
            } else {
                current_block++;
            }   
    }
}
uint8_t ICACHE_FLASH_ATTR _fs_writeblock(fileobject_t *fn, uint8_t block){
    spi_flash_erase_sector(FS_BASE_SECTOR + block);

    //Calculating how many data page we need to store cached data
    uint32_t buffer[FS_PAGE_SIZE / 4] = {[0 ... FS_PAGE_SIZE / 4 - 1] = 0xFFFFFFFF};
    uint8_t pages_len = 1;
    if(fn->cache_len % FS_INDEX_INDEXPERPAGE && fn->cache_len > FS_INDEX_INDEXPERPAGE){
        pages_len += fn->cache_len / FS_INDEX_INDEXPERPAGE + 1;
    } else {
        pages_len += fn->cache_len / FS_INDEX_INDEXPERPAGE;
    }
    //Building index page
    uint8_t i;
    if(pages_len % 2){
        for(i = 0; i < pages_len -1 ; i++){
            buffer[i] = ( fn->objid << 16 | fn->objid) & 0x7FFF7FFF;                // Get data page ID out of meta page id
        }
        buffer[i] = 0xFFFF0000 | (fn->objid & 0x7FFF);                              // OBJID is 2 byte value, so we need to fill up
    } else {
        for(i = 0; i < pages_len -1 ; i++){
            buffer[i] = ( fn->objid << 16 | fn->objid) & 0x7FFF7FFF;
        }
    }
    buffer[FS_PAGE_SIZE / 4 - 1 ] = 0x04390000; //Magic value
    #ifdef FS_DEBUG
    os_printf("\n\rINDEX PAGE\n\r");
    for(i = 0; i < FS_PAGE_SIZE / 4; i++){
        if(i % 8)
            os_printf("0x%08x ",buffer[i]);
        else
            os_printf("\n\r0x%08x ",buffer[i]);
    }
    #endif
    spi_flash_write(FS_BASE_ADDRESS + FS_BLOCK_SIZE * block, buffer, FS_PAGE_SIZE);

    //Building data page
    uint16_t data_index;
    for(data_index = 0 ; data_index < pages_len; data_index++){
        uint32_t buffer[FS_PAGE_SIZE / 4] = {[0 ... FS_PAGE_SIZE / 4 - 1] = 0xFFFFFFFF};
        buffer[0] = ((fn->objid & 0x7FFF) << 16) | data_index;                              //OBJID and SNAP index
        uint8_t counter;
        buffer[1] = 0xFC << 24;                                                             //FLAG
        if(fn->cache_len <= 3){                                                             //align data to 4byte
            for(counter = 0; counter < fn->cache_len ; counter++)                           //DATA
                buffer[1] |= fn->cache[counter] << (16 - counter * 8);
            while(counter < 3)                                                              //Padding
                buffer[1] |= 0xFF << (16 - counter++ * 8);                                    
        } else {
            for(counter = 0; counter < 3 ; counter++)                                       //DATA
                buffer[1] |= fn->cache[counter] << (16 - counter * 8);
        }
        #ifdef FS_DEBUG
        os_printf("\n\rINDEX PAGE\n\r");
        for(i = 0; i < FS_PAGE_SIZE / 4; i++){
            if(i % 8)
                os_printf("0x%08x ",buffer[i]);
            else
                os_printf("\n\r0x%08x ",buffer[i]);
        }
        #endif
        os_printf("\n\rChache[3] %x, address: %x", fn->cache[4], &fn->cache[4]);
        uint32_t *p = (uint32_t *) &fn->cache[4];
        os_printf("\n\r*p address %x", p);   
        os_printf("\n\rChar as int %08x", *p);                                                               
    }



    //Updating filehandler datapage index
    



}