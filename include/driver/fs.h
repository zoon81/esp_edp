/*
File system implementation based on SPIFFS.
BLOCK -> PAGE
PAGE 0: LookUp table for the actual block where OBJID [2byte] of files are listed
        MSB of the objID is set if this is an index page.
        SPECICAL OBJ ID:    00 -- Deleted page
                            FF -- Free page

Index page struckure: [little endian]
    0-1     :   OBJID [2byte]
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