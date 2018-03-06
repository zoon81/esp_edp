#ifndef INC_FS_H
#define INC_FS_H
/*
File system implementation based on SPIFFS.
BLOCK -> PAGE
PAGE 0: LookUp table for the actual block where OBJID [2byte] of files are listed
        MSB of the objID is set if this is an index page.
        SPECICAL OBJ ID:    00 -- Deleted page
                            FF -- Free page

Index page struckure: [little endian]
    0-1     :  OBJID [2byte]
    2-8     :   X
    9-10    :  SIZE of data file [2byte]
    11-12   :   X
    13-44   : FILENAME
    45-     : PAGE Numbers where the content of the file is stored [2byte per page number]

Data page structure: 
    0-1 :   OBJID [2byte]
    2-3   : SNAP_IX [2byte]
    4     : FLAGS
    5-255 : DATA [251byte]

*/

#include <os_type.h>
#include <spi_flash.h>
#include <osapi.h>

#include <mem.h>

#define FS_BASE_ADDRESS 0x63000
#define FS_PAGE_SIZE 256
#define FS_BLOCK_SIZE 4096
#define FS_WHOLE_SIZE 65536
#define FS_META_MAX_FILENAME_LEN 32
#define FS_META_FILENAME_OFFSET 13

#define FS_DEBUG 1

#define UPPER_16(x) ((x) >> 16)
#define LOWER_16(x) ((x) & 0x0000FFFF)

#define UPPER_8(x) ((x) >> 24)
#define UPPER_MID_8(x) ( ((x) & 0x00FF0000) >> 16)
#define LOWER_MID_8(x) ( ((x) & 0x0000FF00) >> 8)
#define LOWER_8(x) ((x) & 0xFF)

typedef struct {
    char filename[FS_META_MAX_FILENAME_LEN];                    //Dynamic length strings may look much better
    uint16_t object_id;
    uint8_t block;
}fs_index_t;

void fs_init();
uint8_t fs_load_index_page(uint8_t indexpage_offset, uint16_t **object_ids);

#endif