#ifndef _IDCT_H_
#define _IDCT_H_
#include <stdint.h>

/*FONCTIONS*/

extern void idct(uint8_t out[8][8],int16_t tab_freq[8][8]);
extern uint8_t S(int x,int y,int16_t tab_freq[8][8]);


#endif /*_IDCT_H_*/