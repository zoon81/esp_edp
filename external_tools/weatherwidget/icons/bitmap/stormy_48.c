
/*******************************************************************************
* image
* filename: unsaved
* name: stormy_48
*
* preset name: Monochrome
* data block size: 8 bit(s), uint8_t
* RLE compression enabled: no
* conversion type: Monochrome, Diffuse Dither 128
* bits per pixel: 1
*
* preprocess:
*  main scan direction: top_to_bottom
*  line scan direction: forward
*  inverse: no
*******************************************************************************/

/*
 typedef struct {
     const uint8_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;
*/
#include <stdint.h>



static const uint8_t image_data_stormy_48[288] = {
    0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 
    0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 
    0xff, 0xff, 0xf0, 0x00, 0x7f, 0xff, 
    0xff, 0xff, 0xe0, 0x00, 0x3f, 0xff, 
    0xff, 0xff, 0xc0, 0x00, 0x1f, 0xff, 
    0xff, 0xff, 0x80, 0x00, 0x1f, 0xff, 
    0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 
    0xff, 0x00, 0x00, 0x00, 0x0f, 0xff, 
    0xfe, 0x00, 0x00, 0x00, 0x07, 0xff, 
    0xfc, 0x00, 0x00, 0x00, 0x07, 0xff, 
    0xf8, 0x00, 0x00, 0x00, 0x07, 0xff, 
    0xf0, 0x00, 0x00, 0x00, 0x00, 0xff, 
    0xf0, 0x00, 0x00, 0x00, 0x00, 0x3f, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x1f, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x0f, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x0f, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 
    0xf0, 0x00, 0x00, 0x00, 0x00, 0x07, 
    0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 
    0xf8, 0x00, 0x00, 0x00, 0x00, 0x0f, 
    0xfc, 0x00, 0x00, 0x00, 0x00, 0x1f, 
    0xfe, 0x00, 0x00, 0x00, 0x00, 0x3f, 
    0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 
    0xff, 0xe0, 0x00, 0x00, 0x01, 0xff, 
    0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 
    0xff, 0xfe, 0xfe, 0x1f, 0xf3, 0xff, 
    0xff, 0xfc, 0xfc, 0x3f, 0xf3, 0xff, 
    0xff, 0xfc, 0x78, 0x00, 0xe3, 0xff, 
    0xff, 0xf8, 0x70, 0x01, 0xe1, 0xff, 
    0xff, 0xf8, 0x60, 0x03, 0xe1, 0xff, 
    0xff, 0xfc, 0x7f, 0x07, 0xe3, 0xff, 
    0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 
    0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 
    0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 
    0xff, 0x7f, 0xfc, 0x7d, 0xff, 0xff, 
    0xfe, 0x7f, 0xf8, 0xf9, 0xff, 0xff, 
    0xfe, 0x3f, 0xf9, 0xf8, 0xff, 0xff, 
    0xfc, 0x3f, 0xf3, 0xf0, 0xff, 0xff, 
    0xfc, 0x3f, 0xf7, 0xf0, 0xff, 0xff, 
    0xfc, 0x3f, 0xef, 0xf0, 0xff, 0xff, 
    0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
const tImage stormy_48 = { image_data_stormy_48, 48, 48,
    8 };
