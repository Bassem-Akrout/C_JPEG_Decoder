#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* EXTRACT BITSTREAM INTO A STRING IN ORDER TO READ BIT BY BIT */

void create_bitstream(FILE* file){
    
    /*CREATES BITSTREAM TEXT*/
    
    FILE* bitstream=fopen("bitstream.txt","wb");
    uint16_t buffer_16;
    while (fread(&buffer_16,sizeof(uint16_t),1,file)==1){
        
        buffer_16=htons(buffer_16);
        if (buffer_16==0xffd9) break;
        for (int i = 0; i < 16; i++) { // loop through each bit of the 16-bit data
            int bit = (buffer_16 >> (15 - i)) & 1; // extract the i-th bit from the data
            fprintf(bitstream, "%d", bit); // write the binary representation of the bit to the file
        }
    }
    fclose(bitstream);
}