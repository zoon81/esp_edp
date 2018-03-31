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
    2-3     :  SNAP IX [2byte]
    4       :  FLAGS
    5-7     : X
    8-9    :  SIZE of file [2byte]
    10-12   :   X
    13-44   : FILENAME
    44-     : OFFSET where the content of the file is stored [2byte per page number]

Data page structure: 
    0-1 :   OBJID [2byte]
    2-3   : SNAP_IX [2byte]
    4     : FLAGS
    5-255 : DATA [251byte]

*/

#include <os_type.h>
#include <spi_flash.h>
#include <osapi.h>
#include <stdlib.h>
#include <mem.h>
#include "m_string.h"

#define FS_BASE_ADDRESS 0x63000
#define FS_PAGE_SIZE 256
#define FS_BLOCK_SIZE 4096
#define FS_WHOLE_SIZE 65536

#define FS_META_MAX_FILENAME_LEN 32
#define FS_META_FILENAME_OFFSET 13
#define FS_META_SIZE_OFFSET 8
#define FS_META_DATAPAGES_OFFSET 44
// This is an "active low" flags
#define FS_FLAG_USED 1
#define FS_FLAG_FILNALIZED 2
#define FS_FLAG_ISMETAPAGE 4
#define FS_FLAG_DELETED 128

#define FS_DATA_DATAPERPAGE 251
#define FS_DATA_DATAOFFSET 5

#define FS_ERR_FILE_NOT_FOUND_BY_NAME -1
#define FS_ERR_FILE_ALLREADY_EXIST -2
#define FS_ERR_FILE_FILENAME_TOO_LONG -3

#define FS_INDEX_FLAGS_FILE_IN_FLASH 0

#define FS_DEBUG 1

#define UPPER_16(x) ((x) >> 16)
#define LOWER_16(x) ((x) & 0x0000FFFF)

#define UPPER_8(x) ((x) >> 24)
#define UPPER_MID_8(x) ( ((x) & 0x00FF0000) >> 16)
#define LOWER_MID_8(x) ( ((x) & 0x0000FF00) >> 8)
#define LOWER_8(x) ((x) & 0xFF)

typedef struct {
    char filename[FS_META_MAX_FILENAME_LEN];              //Dynamic length strings may look much better
    uint16_t object_id;
    uint16_t block;
    uint16_t page;
    uint8_t flags;                                        // bits: [0] -> File are in flash. This is usefull when a new file created and it is not writen to the flash yet.
}fs_index_t;

typedef struct{
    uint16_t objid;
    uint32_t filepointer;
    uint32_t size;
    uint16_t block;
    uint16_t *pages;
    char *cache;                                            // Data witch is not writen to the flash
    uint16_t cache_len;                                     // Size of the cache
} fileobject_t;

extern fs_index_t *fs_index;            // Look-up table for objectids and filenames
extern uint16_t fs_index_size;

void fs_init();
uint8_t ICACHE_FLASH_ATTR _fs_load_index_page(uint8_t indexpage_offset, uint16_t **object_ids);
uint8_t _fs_getfreepages(uint16_t *buffer, uint8_t numberOfFreePages);

#endif