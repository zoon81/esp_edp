#include "driver/fs.h"

void fs_init()
{
    fs_index_t *fs_index = (fs_index_t *)os_malloc(sizeof(fs_index));
    uint16_t *object_ids;
    uint8_t size = fs_load_index_page(0, object_ids);
}

uint8_t fs_load_index_page(uint8_t indexpage_offset, uint16_t *object_ids)
{
    uint32_t obj_id[FS_PAGE_SIZE];
    spi_flash_read((FS_BASE_ADDRESS + indexpage_offset * (uint16_t)FS_PAGE_SIZE), obj_id,(uint16_t)FS_PAGE_SIZE / 4);
    uint16_t index;
    for (index = 0; index < FS_PAGE_SIZE / 4; index++)
    {
        os_printf("\nOBJ ID : %d -> %x", index, obj_id[index]);
    }
    uint16_t size = 0;
    while (obj_id[size] != 0xFFFFFFFF && size < FS_PAGE_SIZE)
    {
        size++;
    }
    os_printf("\nSize of index page : %d", size);
    object_ids = (uint16_t *)os_malloc(sizeof(uint16_t) * size * 2);
    for (index = 0; index < size; index++)
    {
        object_ids[index * 2] = (obj_id[index] & 0xFFFF);
        os_printf("\nOBJ ID : %d -> 0x%x", index, (obj_id[index] & 0xFFFF));
        object_ids[index * 2 + 1] = (obj_id[index] >> 16);
        os_printf("\nOBJ ID : %d -> 0x%x", index, (obj_id[index] >> 16));
    }
    for (index = 0; index < size * 2; index++)
    {
        os_printf("\nOBJ ID : %d -> 0x%x", index, object_ids[index]);
    }
    return size * 2;
}
