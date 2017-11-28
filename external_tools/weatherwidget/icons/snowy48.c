
/*******************************************************************************
* image
* filename: unsaved
* name: snowy48
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



static const uint8_t image_data_snowy48[288] = {
    0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 
    0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 
    0xff, 0xff, 0xf0, 0x00, 0x7f, 0xff, 
    0xff, 0xff, 0xe0, 0x00, 0x3f, 0xff, 
    0xff, 0xff, 0xc0, 0x00, 0x1f, 0xff, 
    0xff, 0xff, 0x80, 0x00, 0x0f, 0xff, 
    0xff, 0xe1, 0x00, 0x00, 0x0f, 0xff, 
    0xff, 0x00, 0x00, 0x00, 0x07, 0xff, 
    0xfc, 0x00, 0x00, 0x00, 0x07, 0xff, 
    0xf8, 0x00, 0x00, 0x00, 0x07, 0xff, 
    0xf0, 0x00, 0x00, 0x00, 0x07, 0xff, 
    0xf0, 0x00, 0x00, 0x00, 0x00, 0xff, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x3f, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x1f, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x0f, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xe0, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xf0, 0x00, 0x00, 0x00, 0x00, 0x07, 
    0xf0, 0x00, 0x00, 0x00, 0x00, 0x07, 
    0xf8, 0x00, 0x00, 0x00, 0x00, 0x0f, 
    0xfc, 0x00, 0x00, 0x00, 0x00, 0x1f, 
    0xff, 0x00, 0x00, 0x00, 0x00, 0x3f, 
    0xff, 0xc0, 0x00, 0x00, 0x00, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 
    0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 
    0xff, 0xf8, 0x0f, 0xf0, 0x1f, 0xff, 
    0xff, 0xfc, 0x1f, 0xf8, 0x3f, 0xff, 
    0xff, 0xf8, 0x0f, 0xf0, 0x1f, 0xff, 
    0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 
    0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 
    0xfd, 0x2f, 0xfa, 0x5f, 0xf4, 0xbf, 
    0xfc, 0x07, 0xf8, 0x0f, 0xf0, 0x3f, 
    0xfc, 0x0f, 0xf8, 0x1f, 0xf0, 0x3f, 
    0xfc, 0x07, 0xf0, 0x0f, 0xf0, 0x3f, 
    0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 
    0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff
};
const tImage snowy48 = { image_data_snowy48, 48, 48,
    8 };
