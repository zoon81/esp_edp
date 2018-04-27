#include "driver/fs.h"

uint8_t ICACHE_FLASH_ATTR _fs_load_index_page(uint8_t body_offset, uint16_t **object_ids){
    uint32_t obj_id[FS_PAGE_SIZE / 4];
    spi_flash_read((FS_BASE_ADDRESS + body_offset * (uint16_t)FS_BLOCK_SIZE), obj_id, (uint16_t)FS_PAGE_SIZE);
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

uint8_t ICACHE_FLASH_ATTR _fs_getfreepages(uint16_t *body, uint8_t numberOfFreePages){
    uint16_t freePageOffset = 0;
    uint8_t current_block = 0;
    uint8_t freepage_cache_index = 0;
    while (freepage_cache_index < numberOfFreePages){
        do{
            uint16_t *object_ids;
            uint8_t numberOfIDs = _fs_load_index_page(current_block, &object_ids);
            uint8_t cache_index = 0;
            while (cache_index < numberOfIDs){
                #ifdef FS_DEBUG
                os_printf("\nFree page finder on block %d OBJ_ID 0x%04x", current_block, object_ids[cache_index]);
                #endif
                if (object_ids[cache_index] == 0x0000 | object_ids[cache_index] == 0xFFFF){
                    freePageOffset = current_block * FS_BLOCK_SIZE + (cache_index + 1) * FS_PAGE_SIZE;
                    cache_index = numberOfIDs;
                }else{
                    cache_index++;
                }
            }
            current_block++;
        } while (!freePageOffset);
        body[freepage_cache_index] = freePageOffset;
        freepage_cache_index++;
    }
    #ifdef FS_DEBUG
    uint8_t i;
    os_printf("\nFree page offset:");
    for(i = 0; i < freepage_cache_index; i++){
        os_printf("0x%04x\t", body[i]);
    }
    #endif
}
void ICACHE_FLASH_ATTR _fs_dump_fsindex(){
    uint16_t cache_index = 0;
    os_printf("FS_INDEX_DUMP");
    while(cache_index < fs_index_size ){
        os_printf("Filename: %s OBJID: %04x block: %d page: %d flags 0x%02x", fs_index[cache_index].filename, fs_index[cache_index].object_id, fs_index[cache_index].block, fs_index[cache_index].page, fs_index[cache_index].flags);
        cache_index++;
    }
}
void ICACHE_FLASH_ATTR _fs_dump_fileobject(fileobject_t *fn){
    os_printf("\n\rOBJID: %04x\tfilepointer: %d\tSize: %d\tBlock: %d\tCache address: 0x%04x\t Cache len: %d", fn->objid, fn->filepointer, fn->size, fn->block, &fn->cache[0], fn->cache_len);
    // Calculate how many data page we have based on its size and print it out
    
    os_printf("\n\rNumber of datapages: %d\n\rDatapage_index:\n\r", fn->pages_len);
    uint16_t cache_index;
    for(cache_index = 0; cache_index < fn->pages_len; cache_index++){
        if(cache_index % 6)
            os_printf("%04x, ", fn->pages[cache_index]);
        else
            os_printf("\n%04x, ", fn->pages[cache_index]);
    }
    os_printf("\n\rCache:\n\r");
    for(cache_index = 0; cache_index < fn->cache_len; cache_index++){
        if(cache_index % 16)
            os_printf("%02x, ", fn->cache[cache_index]);
        else
            os_printf("\n%02x, ", fn->cache[cache_index]);
    }
}
uint8_t ICACHE_FLASH_ATTR _fs_getfreeblock(){
    #ifdef FS_DEBUG
    os_printf("\n\rFreeblock finder\n\r");
    #endif
    uint8_t current_block = 0;
    while(current_block < FS_WHOLE_SIZE / FS_BLOCK_SIZE){
        uint32_t body[FS_PAGE_SIZE / 4];
        spi_flash_read(FS_BASE_ADDRESS + current_block * FS_BLOCK_SIZE, body, FS_PAGE_SIZE);
        uint8_t cache_index = 0;
        #ifdef FS_DEBUG
        os_printf("Checking block %d", current_block);
        #endif
        while(cache_index < FS_PAGE_SIZE / 4 - 1){             //the last dword in the index page has different meaning
            #ifdef FS_DEBUG
            if(! (cache_index % 6) )
                os_printf("\n\r");
            os_printf("%02d->0x%08x,", cache_index, body[cache_index]);
            #endif
            if(body[cache_index] == 0x00000000 | body[cache_index] == 0xFFFFFFFF){
                cache_index++;
            } else {
                cache_index = 0xFF;                             // This means this is a used block
                #ifdef FS_DEBUG
                os_printf("\n\rThis block are used\n\r");
                #endif
            }
        }
        if(cache_index == (FS_PAGE_SIZE / 4 - 1)){
            return current_block;
        } else if( (current_block == FS_WHOLE_SIZE / FS_BLOCK_SIZE -1) && cache_index == 0xFF) {
             return current_block = 0xff;                           // No more free block on the filesystem
            } else {
                current_block++;
            }   
    }
}

//This function erase the whole block and write it. It is useful when we have a lot data to write out.
uint8_t ICACHE_FLASH_ATTR _fs_createFullBlock(fileobject_t *fn, uint8_t block){
    spi_flash_erase_sector(FS_BASE_SECTOR + block);

    //Calculating how many data page we need to store cached data
    uint32_t body[FS_PAGE_SIZE / 4] = {[0 ... FS_PAGE_SIZE / 4 - 1] = 0xFFFFFFFF};
    uint8_t pages_len = 1;
    if(fn->cache_len % FS_INDEX_INDEXPERPAGE){
        pages_len = (uint8_t)(fn->cache_len / FS_INDEX_INDEXPERPAGE) + 1;
    } else {
        pages_len += fn->cache_len / FS_INDEX_INDEXPERPAGE;
    }
    #ifdef FS_DEBUG
    os_printf("\n\rNeed %d page to store data\n\r", pages_len);
    #endif
    //Building index page
    uint8_t i;
    if(pages_len % 2){
        for(i = 0; i < pages_len / 2 ; i++){
            body[i] = ( fn->objid << 16 | fn->objid) & 0x7FFF7FFF;                // Get data page ID out of meta page id
        }
        body[i] = 0xFFFF0000 | (fn->objid & 0x7FFF);                              // OBJID is 2 byte value, so we need to fill up
    } else {
        for(i = 0; i < pages_len / 2; i++){
            body[i] = ( fn->objid << 16 | fn->objid) & 0x7FFF7FFF;
        }
    }
    body[FS_PAGE_SIZE / 4 - 1 ] = 0x04390000; //Magic value
    #ifdef FS_DEBUG
    os_printf("\n\rINDEX PAGE\n\r");
    for(i = 0; i < FS_PAGE_SIZE / 4; i++){
        if(i % 8)
            os_printf("0x%08x ",body[i]);
        else
            os_printf("\n\r0x%08x ",body[i]);
    }
    #endif
    spi_flash_write(FS_BASE_ADDRESS + FS_BLOCK_SIZE * block, body, FS_PAGE_SIZE);

    //Building data page
    uint16_t page_index;
    uint16_t cache_index = 0;
    for(page_index = 0 ; page_index < pages_len; page_index++){
        uint32_t body[FS_PAGE_SIZE / 4] = {[0 ... FS_PAGE_SIZE / 4 - 1] = 0xFFFFFFFF};
        body[0] = (page_index << 16) | (fn->objid & 0x7FFF);                                              //OBJID and SNAP index
        body[1] = 0xFC;
        uint8_t cache_offset;                                                                               //Combine data and FLAG at the second dword
        if((fn->cache_len - cache_index) < 3){                                                              //align data to 4byte
            for(cache_offset = 0; cache_offset < (fn->cache_len - cache_index) ;)                           //DATA
                body[1] |= fn->cache[cache_index + cache_offset] << (++cache_offset * 8);
            while(cache_offset < 3)                                                                         //Padding
                body[1] |= 0xFF << (++cache_offset * 8);                                    
        } else {
            for(cache_offset = 0; cache_offset < 3 ; )                                                      //just DATA, no padding required here
                body[1] |= fn->cache[cache_index + cache_offset] << (++cache_offset * 8);
        }
        uint8_t body_index = 2;
        cache_index += cache_offset;                                                                        //Copy remeaning data to body
        while((body_index <= (FS_DATA_DATAPERPAGE / 4 + 1)) && (cache_index < fn->cache_len) ){
            if((fn->cache_len - cache_index) / 4 ){
                body[body_index] = (uint32_t) fn->cache[cache_index + 3] << 24 | fn->cache[cache_index + 2] << 16 | fn->cache[cache_index + 1] << 8 | fn->cache[cache_index];
                cache_index += 4;
            } else {                                                                                       //align data to 4byte
                while(fn->cache_len - cache_index){
                    char tmp = ~fn->cache[cache_index];
                    body[body_index] &= ~( tmp << (((cache_index + 1) % 4) * 8) );
                    cache_index++;
                }
            }
            body_index++;
        }
        #ifdef FS_DEBUG
        os_printf("\n\r\n\rData Page %d, Cache index: %d", page_index, cache_index);
        for(i = 0; i < FS_PAGE_SIZE / 4; i++){
            if(i % 8)
                os_printf("0x%08x ",body[i]);
            else
                os_printf("\n\r0x%08x ",body[i]);
        }
        #endif
        spi_flash_write(FS_BASE_ADDRESS + FS_BLOCK_SIZE * block + FS_PAGE_SIZE * (page_index + 1), body, FS_PAGE_SIZE);                                                         
    }

    // Updating fileobject
    if(fn->pages == NULL ){
        fn->pages = (uint16_t *) os_malloc(sizeof(fn->pages) * pages_len);
    } else {
        fn->pages = (uint16_t *) os_realloc(fn->pages, fn->pages_len + sizeof(fn->pages) * pages_len);
    }
    for(page_index = 0 ; page_index < pages_len; page_index++)
        fn->pages[fn->pages_len + i] = FS_BLOCK_SIZE * block + FS_PAGE_SIZE * (page_index + 1);
    fn->size += fn->cache_len;
    fn->pages_len = pages_len;

    // Freeing up file cache
    os_free(fn->cache);
    fn->cache_len = 0;

    #ifdef FS_DEBUG
    _fs_dump_fileobject(fn);
    #endif

    //This is better solution for base conversion but who the crab can set a pointer to the 3. element of an array..
    //Sould I drop the SPIFFS compatibility, and make my own faster and better version for the esp8266
    // os_printf("\n\rChache[3] %x, address: %x", fn->cache[4], &fn->cache[4]);
    // uint32_t *p = (uint32_t *) &fn->cache[4];
    // os_printf("\n\r*p address %x", p);   
    // os_printf("\n\rChar as int %08x", *p);   

}
uint8_t  _fs_writePage(uint16_t page_start_addr, char *header, uint8_t header_len, char *body, uint8_t body_len, uint32_t footer){
    os_printf("\n\rPAGEWRITE\n\r");
    uint8_t block = page_start_addr / FS_BLOCK_SIZE;
    uint32_t *block_body = (uint32_t *) os_malloc( sizeof(uint32) * FS_BLOCK_SIZE / 4);
    uint16_t block_body_index = 0;
    spi_flash_read(FS_BASE_ADDRESS + FS_BLOCK_SIZE * block, block_body, FS_BLOCK_SIZE);
    //spi_flash_erase_sector(FS_BASE_SECTOR + block);
    uint8_t i = 0,j;
    //header
    while((header_len - i) / 4){
        block_body[page_start_addr + block_body_index] = (uint32_t) header[i + 3 ] << 24 | header[i + 2 ] << 16 | header[i + 1 ] << 8 | header[i];
        block_body_index++;
        i+=4;
    }
    block_body[page_start_addr + block_body_index] = 0;
    //mixing header and data
    while(i < header_len){
        block_body[page_start_addr + block_body_index] |= header[i] << ((i % 4) * 8); 
        i++;
    }
    j = 0;
    while( i % 4 ){
        block_body[page_start_addr + block_body_index] |= body[j] << ((i % 4) * 8);
        i++;
        j++;
    }
    #ifdef FS_DEBUG
    os_printf("\n\rIndex: %d Page: \n\r", page_start_addr + block_body_index);
    for(i = 0; i < FS_PAGE_SIZE / 4 ; i++){
        if(i % 4)
            os_printf("%08x ", block_body[i]);
        else
            os_printf("\n\r%08x ", block_body[i]);
    }
    #endif

    //body
    block_body_index++;
    i = j;
    while((body_len - i) / 4) {
        block_body[page_start_addr + block_body_index] = (uint32_t) body[i + 3 ] << 24 | body[i + 2 ] << 16 | body[i + 1 ] << 8 | body[i];
        block_body_index++;
        i += 4;
    }
    block_body[page_start_addr + block_body_index] = 0xFFFFFFFF;
    while(i < body_len){
        char tmp = ~body[i];
        block_body[page_start_addr + block_body_index] &= ~( tmp << (((i + 1) % 4) * 8) );
        i++;
    }
   
    block_body_index++;
    if(footer == 0){
        while(block_body_index < FS_PAGE_SIZE  / 4){
            block_body[page_start_addr + block_body_index] = 0xFFFFFFFF;
            block_body_index++;
        }

    } else {
        while(block_body_index < FS_PAGE_SIZE  / 4 - 1){
            block_body[page_start_addr + block_body_index] = 0xFFFFFFFF;
            block_body_index++;
        }
         //footer
        block_body[page_start_addr + block_body_index] = footer;
    }

    
    

    #ifdef FS_DEBUG
    os_printf("\n\rIndex: %d Page: \n\r", page_start_addr + block_body_index);
    for(i = 0; i < FS_PAGE_SIZE / 4 ; i++){
        if(i % 4)
            os_printf("%08x ", block_body[i]);
        else
            os_printf("\n\r%08x ", block_body[i]);
    }
    #endif

    //spi_flash_write(FS_BASE_ADDRESS + FS_BLOCK_SIZE * block, block_body, FS_BLOCK_SIZE);
    os_printf("\n\rPAGEWRITE DONE\n\r");

}

uint8_t ICACHE_FLASH_ATTR _fs_buildMetaPage(fileobject_t *fn, uint32_t *body){
    body[0] = fn->objid;                                                          //OBJ_ID and SNAPIX
    body[1] = 0xF8;                                                               //Fixed value
    body[2] = fn->size;                                                           //Size of a file
    //Updating filename
    uint16_t index = _fsindex_getIndexbyID(fn->objid);
    if(index == fs_index_size){                                                     // OBJID not found in FS_INDEX
        return 1;
    }
    uint8_t filename_len = strlen(fs_index[index].filename);
    uint8_t fileobj_index;
    uint8_t body_index = 4;
    if(filename_len > 2){
        body[3] = fs_index[index].filename[2] << 24 | fs_index[index].filename[1] << 16 | fs_index[index].filename[0] << 8 | 0x01;
        fileobj_index = 3;
        while(fileobj_index < filename_len){
            if((filename_len - fileobj_index) / 4 ){
                body[body_index] = (uint32_t) fs_index[index].filename[fileobj_index + 3] << 24 | fs_index[index].filename[fileobj_index + 2] << 16 | fs_index[index].filename[fileobj_index + 1] << 8 | fs_index[index].filename[fileobj_index];
                fileobj_index += 4;
            } else {
                body[body_index] = 0;
                while(fileobj_index < filename_len){
                    body[body_index] |= fs_index[index].filename[fileobj_index] << ((fileobj_index % 4 - 1) * 8);
                    fileobj_index++;
                }
            }
        body_index++;
        }
    } else {
        body[3] = fs_index[index].filename[1] << 16 | fs_index[index].filename[0] << 8 | 0x01;
    }
    while(body_index < FS_META_MAX_FILENAME_LEN / 4)
        body[body_index++] = 0x0;
    //Update data page indexes
    for(index = 0; index < fn->pages_len; index+=2){
        body[body_index++] = fn->pages[index + 1] << 16 | fn->pages[index];
    }
    if(index != fn->pages_len){
        body[body_index++] = 0xFFFF << 16 | fn->pages[index];
    }

    //Fill up remaining space
    while(body_index < FS_PAGE_SIZE / 4){
        body[body_index++] = 0xFFFFFFFF;
    }
    #ifdef FS_DEBUG
    os_printf("\n\rMetapage: \n\r");
    for(index = 0; index < FS_PAGE_SIZE / 4 ; index++){
        if(index % 4)
            os_printf("%08x ", body[index]);
        else
            os_printf("\n\r%08x ", body[index]);
    }
    #endif
}
uint16_t _fsindex_getIndexbyID(uint16_t objid){
    uint16_t index = 0;
    while(fs_index[index].object_id != objid && index < fs_index_size){
        index++;
    }
    return index;
}