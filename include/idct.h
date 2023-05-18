#ifndef _IDCT_H_
#define _IDCT_H_
#include <stdint.h>

/*FONCTIONS*/

extern uint8_t S(int x,int y,int16_t*** tab_freq);
extern void idct(uint8_t*** out,int16_t*** tab_freq);



#endif /*_IDCT_H_*/