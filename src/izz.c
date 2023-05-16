#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int16_t **zz_inverse(int16_t* tab_freq){
    int i,j=0;
    int16_t** mat_zz_inverse=malloc(sizeof(*mat_zz_inverse)*8);
    for (int i=0;i<8;i++){
        mat_zz_inverse[i]=malloc(sizeof(int16_t)*8);
    }
    int index=0;
    int i_change=0;
    int j_change=0;
    while (index<64){
        i+=i_change;
        j+=j_change;
        mat_zz_inverse[i][j]=tab_freq[index];
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
            mat_zz_inverse[i][j]=tab_freq[index];
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
            mat_zz_inverse[i][j]=tab_freq[index];
            index++;
        }

    }
    return mat_zz_inverse;
}

/* AFTER ZIGZAG, ASSIGN ELEMENTS TO NEW MCU CONTAINING BLOCKS OF MATRIX */

