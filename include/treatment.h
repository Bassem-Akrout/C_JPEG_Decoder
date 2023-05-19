#ifndef _TREATMENT_H_
#define _TREATMENT_H_
#include "izz.h"


/*FONCTIONS*/
extern void create_M_MCU(M_MCU* new_M_MCU  ,MCU* mcu,uint8_t* occ_list,struct DQTs* DQT_list,uint8_t components_number,struct SOF* sof);
extern M_LMCU* create_M_LMCU(LMCU* lmcu,struct DQTs* DQT_list,uint8_t components_number,struct SOF* sof);

#endif /*_TREATMENT_H_*/