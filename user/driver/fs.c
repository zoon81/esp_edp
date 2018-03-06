#include "driver/fs.h"

fs_index_t *fs_index = NULL; // Look-up table for objectid and filename
uint16_t fs_index_size = 0;
uint16_t *object_ids; // cached index page

void ICACHE_FLASH_ATTR fs_init()
{
    uint8_t block;
    for (block = 0; block < (FS_WHOLE_SIZE / FS_BLOCK_SIZE); block++)
    {
        uint8_t numberOfnewID = fs_load_index_page(block, &object_ids);       //Here we store the number of new ObjIDs that we found on the current block
        fs_index_size += numberOfnewID;
        uint16_t index;
#if FS_DEBUG
        for (index = 0; index < numberOfnewID; index++)
        {
            os_printf("\nOBJ ID : %d -> 0x%x", index, object_ids[index]);
        }
#endif
        if(fs_index){
            fs_index = (fs_index_t *)os_malloc(sizeof(fs_index_t) * fs_index_size);
        } else {
            fs_index = (fs_index_t *)os_realloc(fs_index, sizeof(fs_index_t) * fs_index_size);
        }

        for (index = fs_index_size - numberOfnewID; index < fs_index_size; index++)
        {
            //get only meta pages
            if (object_ids[index] & 0x8000)
            {
                uint32_t buffer_objid;
                spi_flash_read(FS_BASE_ADDRESS + (block * FS_BLOCK_SIZE) + ((uint16_t)FS_PAGE_SIZE * (index + 1)), &buffer_objid, 4);
#if FS_DEBUG
                os_printf("\nOBJ_ID: %x", buffer_objid);
#endif
                fs_index[index].object_id = (uint16_t)(buffer_objid & 0xFFFF);
                uint32_t buffer_filename[FS_META_MAX_FILENAME_LEN / 4];
                spi_flash_read(FS_BASE_ADDRESS + (block * FS_BLOCK_SIZE) + ((uint16_t)FS_PAGE_SIZE * (index + 1)) + FS_META_FILENAME_OFFSET, buffer_filename, FS_META_MAX_FILENAME_LEN);
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
                fs_index[index].block = block;
#if FS_DEBUG
                os_printf("\nCurernt block: %d", fs_index[index].block);
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
        os_printf("\nOBJ ID : %d -> %x", index, obj_id[index * 4 + 0]);
        os_printf("\tOBJ ID : %d -> %x", index, obj_id[index * 4 + 1]);
        os_printf("\tOBJ ID : %d -> %x", index, obj_id[index * 4 + 2]);
        os_printf("\tOBJ ID : %d -> %x", index, obj_id[index * 4 + 3]);
    }
#endif
    uint16_t size = 0;
    while (obj_id[size] != 0xFFFFFFFF && size < FS_PAGE_SIZE)
    {
        size++;
    }
    *object_ids = (uint16_t *)os_malloc(sizeof(uint16_t) * size * 2);
    for (index = 0; index < size; index++)
    {
        *(*object_ids + index * 2) = LOWER_16(obj_id[index]);
        *(*object_ids + index * 2 + 1) = UPPER_16(obj_id[index]);
    }
    return size * 2;
}
