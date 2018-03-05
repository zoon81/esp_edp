#include "driver/fs.h"

fs_index_t *fs_index;                // Look-up table for objectid and filename
uint16_t *object_ids;        // cached index page

void ICACHE_FLASH_ATTR fs_init()
{
    uint8_t size = fs_load_index_page(0, &object_ids);
    uint8 index;
    #if FS_DEBUG
    for (index = 0; index < size; index++)
    {
        os_printf("\nOBJ ID : %d -> 0x%x", index, object_ids[index]);
    }
    #endif
    fs_index = (fs_index_t *)os_malloc(sizeof(fs_index_t) * size);
    for (index = 0; index < size; index++)
    {
        if (object_ids[index] & 0x8000)
        {
            uint32_t buffer_objid;
            spi_flash_read(FS_BASE_ADDRESS + ((uint16_t)FS_PAGE_SIZE * (index + 1)), &buffer_objid, 4);
            #if FS_DEBUG
            os_printf("\nOBJ_ID: %x", buffer_objid);
            #endif
            fs_index[index].object_id = (uint16_t)(buffer_objid & 0xFFFF);
            uint32_t buffer_filename[FS_META_MAX_FILENAME_LEN / 4];
            spi_flash_read(FS_BASE_ADDRESS + ((uint16_t)FS_PAGE_SIZE * (index + 1)) + FS_META_FILENAME_OFFSET, buffer_filename, FS_META_MAX_FILENAME_LEN);
            uint8 i;
            for (i = 0; i < FS_META_MAX_FILENAME_LEN / 4; i++)
            {
                #if FS_DEBUG
                os_printf("\nFilename: %x", buffer_filename[i]);
                #endif
                fs_index[index].filename[i * 4] = LOWER_8(buffer_filename[i]);
                fs_index[index].filename[i * 4 + 1] = LOWER_MID_8(buffer_filename[i]);
                fs_index[index].filename[i * 4 + 2] = UPPER_MID_8(buffer_filename[i]);
                fs_index[index].filename[i * 4 + 3] = UPPER_8(buffer_filename[i]);
            }
            #if FS_DEBUG
            os_printf("\nDecoded Filename: %s", fs_index[index].filename);
            #endif
        }
    }
}

uint8_t ICACHE_FLASH_ATTR fs_load_index_page(uint8_t indexpage_offset, uint16_t **object_ids)
{
    uint32_t obj_id[FS_PAGE_SIZE / 4];
    spi_flash_read((FS_BASE_ADDRESS + indexpage_offset * (uint16_t)FS_BLOCK_SIZE), obj_id, (uint16_t)FS_PAGE_SIZE);
    uint16_t index;
    #if FS_DEBUG
    for (index = 0; index < FS_PAGE_SIZE / 4; index++)
    {
        os_printf("\nOBJ ID : %d -> %x", index, obj_id[index]);
    }
    #endif
    uint16_t size = 0;
    while (obj_id[size] != 0xFFFFFFFF && size < FS_PAGE_SIZE)
    {
        size++;
    }
    #if FS_DEBUG
    os_printf("\nSize of index page : %d", size);
    #endif
    *object_ids = (uint16_t *)os_malloc(sizeof(uint16_t) * size * 2);
    #if FS_DEBUG
    os_printf("\nObject_ids address: %d", *object_ids);
    #endif
    for (index = 0; index < size; index++)
    {
        *(*object_ids + index * 2) = LOWER_16(obj_id[index]);
        #if FS_DEBUG
        os_printf("\nOBJ ID : %d -> 0x%x", index, *(*object_ids + index));
        #endif
        *(*object_ids + index * 2 + 1) = UPPER_16(obj_id[index]);
        #if FS_DEBUG
        os_printf("\nOBJ ID : %d -> 0x%x", index, *(*object_ids + index + 1));
        #endif
    }
    return size * 2;
}
