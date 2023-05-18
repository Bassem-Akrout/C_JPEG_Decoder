#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/bitstream_to_mcu.h"
#include "../include/izz.h"


void zz_inverse(int16_t*** mat_zz_inverse,int16_t** tab_freq/*quantif table*/){

    // for i in range 64 modif coef (quantification)
    /*for (int i=0;i<64;i++){
        *(tab_freq)[i]*=quantification_matrix[i];*/
    int i=0;
    int j=0;
    int index=0;
    int i_change=0;
    int j_change=0;
    while (index<64){
        i+=i_change;
        j+=j_change;
        *(mat_zz_inverse)[i][j]=*(tab_freq)[index];
        index++;
        if (i==0 || j==7){
            /*CHANGE DIRECTION IF WE FIND LEDGE*/
            if (i==0){
                j++;
            }
            else {
                i++;
            }
            j_change=-1;
            i_change=1;
            *(mat_zz_inverse)[i][j]=*(tab_freq)[index];
            index++;
        }
        else if (i==7 || j==0){
            /*CHANGE DIRECTION AGAIN*/
            if (i==7){
                j++;
            }
            else {
                i++;
            }
            j_change=1;
            i_change=-1;
            *(mat_zz_inverse)[i][j]=*(tab_freq)[index];
            index++;
        }

    }
    
}
M_block* create_M_block(block*  block/*correct dqt table y/cb/cr*/  ){
    


    M_block* new_M_Block = malloc(sizeof(M_block));
    new_M_Block->block_type = block->block_type;
    new_M_Block->content = calloc(8,sizeof(int16_t**));
    for (int i = 0; i < 8; i++) {
        new_M_Block->content[i]=calloc(8,sizeof(int16_t*));
        for (int j = 0; i < 8; j++) {
            new_M_Block->content[i][j]=calloc(8,sizeof(int16_t));
        }
        
    }
    zz_inverse(new_M_Block->content,block->content );
    free(block->content);
    free(block);
    return new_M_Block;
}


/* AFTER ZIGZAG, ASSIGN ELEMENTS TO NEW MCU CONTAINING BLOCKS OF MATRIX */

