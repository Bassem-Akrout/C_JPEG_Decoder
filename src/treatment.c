#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/izz.h"
#include "../include/bitstream_to_mcu.h"


M_MCU* create_M_MCU(MCU* mcu,uint8_t* occ_list){



    M_MCU* new_M_MCU = malloc(sizeof(M_MCU))
        for (uint8_t i=0; i<occ_list[0];i++){
            new_M_MCU->LY[i] = create_M_block(mcu->LY[i]/*dqt_ Y */);           
        }
        for (uint8_t i=0; i<occ_list[1];i++){
            new_M_MCU->LCb[i] = create_M_block(mcu->LCb[i]);            
        }
        for (uint8_t i=0; i<occ_list[2];i++){
            new_M_MCU->LCr[i] = create_M_block(mcu->LCr[i]);
        }
    return new_M_MCU;
    }

M_LMCU* create_M_LMCU(LMCU* lmcu,/*DQTs*/){
    M_LMCU* new_M_LMCU = malloc(sizeof(M_LMCU));



    for(uint32_t i=0; i<lmcu->MCU_counter ;i++){
        new_M_LMCU->M_MCUs[i]=create_M_MCU(lmcu->MCUs[i],lmcu->occurrence_list);
        free(lmcu->MCUs[i])


    }
    new_M_LMCU->occurrence_list=lmcu->occurrence_list;
    new_M_LMCU->order_list=lmcu->order_list;
    new_M_LMCU->MCU_counter=lmcu->MCU_counter;
    free(lmcu->MCUs);
    return new_M_LMCU;
}



