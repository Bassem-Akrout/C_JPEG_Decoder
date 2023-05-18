#ifndef _DECODE_IMAGE_CREATOR_H_
#define _DECODE_IMAGE_CREATOR_H_
#include "jpeg_reader.h"

/*FUNCTIONS*/
    // FOR PGM 
extern void blackandwhite(struct SOF* sof,char* jpeg_name);

    // FOR PPM
extern void rainbow(struct SOF* sof,char* jpeg_name);

#endif /*_DECODE_IMAGE_CREATOR_H_*/