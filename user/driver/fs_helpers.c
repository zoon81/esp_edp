#include "driver/fs.h"

uint8_t ICACHE_FLASH_ATTR _fs_load_index_page(uint8_t indexpage_offset, uint16_t **object_ids){
    uint32_t obj_id[FS_PAGE_SIZE / 4];
    spi_flash_read((FS_BASE_ADDRESS + indexpage_offset * (uint16_t)FS_BLOCK_SIZE), obj_id, (uint16_t)FS_PAGE_SIZE);
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

uint8_t _fs_getfreepages(uint16_t *buffer, uint8_t numberOfFreePages){
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
void _fs_dump_fsindex(){
    uint16_t counter = 0;
    os_printf("FS_INDEX_DUMP");
    while(counter < fs_index_size ){
        os_printf("Filename: %s OBJID: %04x block: %d page: %d flags 0x%02x", fs_index[counter].filename, fs_index[counter].object_id, fs_index[counter].block, fs_index[counter].page, fs_index[counter].flags);
        counter++;
    }
}