#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* THIS FUNCTION UPSAMPLES AN MCU: BY DUPLICATION ! */

/*void upsample_mcu(struct MCU* mcu,uint8_t* hv/* SAMPLING FACTORS ){
    if (hv[0]*hv[1]==hv[2]*hv[3]==hv[4]*hv[5]){
         NO NEED TO UPSAMPLE BECAUSE THERE WAS NO DOWNSAMPLING 
        return;
    }
    else {
        uint8_t nb_blocks_Y=hv[0]*hv[1];
        struct block** new_Cb=calloc(nb_blocks_Y,sizeof(struct block*));
        struct block** new_Cr=calloc(nb_blocks_Y,sizeof(struct block*));
        duplicate(Cb dans new Cb);
        duplicate(Cr dans new Cr);            
    }
 }

    


*/