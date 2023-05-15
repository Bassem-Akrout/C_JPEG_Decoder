#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t SATURATED(float val){
    if (val < 0){ return 0; }
    if (val > 255){ return 255; }
    else return (uint8_t) floor(val+0.5);
}

void one_YCbCr_mcu_to_rgb(struct mcu* mcu_ycbcr,uint32_t* mcu_rgb,uint32_t nb_horz_blocks_in_mcu,uint32_t nb_vertical_blocks_in_mcu){
    uint32_t nb_blocks=nb_horz_blocks_in_mcu*nb_vertical_blocks_in_mcu;
    Y= struct bloc*;
    Cb= struct bloc*;
    Cr =struct bloc*;
    int32_t R,G,B;
    for (uint32_t i=0;i<nb_blocks;i++){
        for (uint32_t j=0;j<8;i++){
            for (uint32_t k=0;k<8;k++){
            R = Y[i][j][k] - 0.0009267 * (Cb[i][j][k] - 128) + 1.4016868 * (Cr[i][j][k] - 128);
            G = Y[i][j][k] - 0.3436954 * (Cb[i][j][k] - 128) - 0.7141690 * (Cr[i][j][k] - 128);
            B = Y[i][j][k] + 1.7721604 * (Cb[i][j][k] - 128) + 0.0009902 * (Cr[i][j][k] - 128);
            mcu_rgb[i]=SATURATED(R)<<16 | SATURATED(G)<<8 | SATURATED(B) ;      
            }
        }
    }
}

void one_Y_mcu_to_rgb(struct mcu* mcu_Y,uint32_t* mcu_rgb,uint32_t nb_horz_blocks_in_mcu,uint32_t nb_vertical_blocks_in_mcu){
        const uint32_t nb_blocks = nb_horz_blocks_in_mcu * nb_vertical_blocks_in_mcu;
        uint8_t g;
        uint8_t Y[1][8][8]={{{0,0,0,255,255,0,0,0},{0,0,255,255,255,255,0,0},{0,255,255,255,255,255,255,0},{255,255,0,255,255,0,255,255},{255,255,255,255,255,255,255,255},{0,0,255,0,0,255,0,0},{0,255,0,255,255,0,255,0},{255,0,255,0,0,255,0,255}}};
        /* Convert MCUs to RGB using Y MCUs */
        for (uint32_t i = 0; i < nb_blocks; i++) {
            for (uint32_t j=0;j<8;j++){
                for (uint32_t k=0;k<8;k++){
                /* Extract RGB values from Y values */
                    g = Y[i][j][k];
                    
                    printf("%u %u %u\n",g<<16,g<<8,g);
                    mcu_rgb[i*64 + j*8 + k] = g << 16 | g << 8 | g;
        
        }
}
}
}

int main(void){
    uint32_t* mcu_rgb=malloc(64*sizeof(uint32_t));
    one_Y_mcu_to_rgb(mcu_rgb,1,1);

    return 0;
}