#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


void quantification(int16_t tab_freq[64],uint8_t quantification_matrix[64]){
    for (int i=0;i<64;i++){
        tab_freq[i]*=quantification_matrix[i];
    }
}


