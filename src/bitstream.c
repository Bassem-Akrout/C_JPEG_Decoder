#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>



/* EXTRACT BITSTREAM INTO A STRING IN ORDER TO READ BIT BY BIT */

void create_stream(FILE* file){
    
    /*CREATES BITSTREAM TEXT AND ELIMINATE BYTESTUFFING*/
    
    FILE* bitstream=fopen("bitstream.txt","wb");
    uint8_t byte, next_byte;
    bool last_byte_ff = false;
    while (fread(&byte, sizeof(uint8_t), 1, file) == 1){
        if (byte == 0xff){
            if (fread(&next_byte, sizeof(uint8_t), 1, file) == 1){
                if (next_byte == 0x00){
                    /* Byte stuffing found, skip 0x00 and write only 0xff */
                    last_byte_ff = true;
                    continue;
                } else if (next_byte == 0xd9){
                    /* End of image marker found, stop processing*/
                    break;
                } else {
                    /* No byte stuffing, write both bytes to output*/
                    if (last_byte_ff) {
                    
                        /*Write the bits in the .txt file*/
                        for (int i = 0; i < 8; i++) {
                            int bit = (0xff >> (7 - i)) & 1;
                            fprintf(bitstream, "%d", bit);
                        }
                       
                        last_byte_ff = false;
                    }
                   
                    /*Write the bits in the .txt file*/
                    for (int i = 0; i < 8; i++) {
                        int bit = (byte >> (7 - i)) & 1;
                        fprintf(bitstream, "%d", bit);
                    }
                   
                }
            } else {
                /* End of file reached, write last byte to output */
               
                /*Write the bits in the .txt file*/
                for (int i = 0; i < 8; i++) {
                    int bit = (byte >> (7 - i)) & 1;
                    fprintf(bitstream, "%d", bit);
                }
               
                break;
            }
        } else {
            /* No byte stuffing, write byte to output*/
            if (last_byte_ff) {
               
                /*Write the bits in the .txt file*/
                for (int i = 0; i < 8; i++) {
                    int bit = (0xff >> (7 - i)) & 1;
                    fprintf(bitstream, "%d", bit);
                }
               
                last_byte_ff = false;
            }
            
            /*Write the bits in the .txt file*/
            for (int i = 0; i < 8; i++) {
                int bit = (byte >> (7 - i)) & 1;
                fprintf(bitstream, "%d", bit);
            }
           
        }
    }
    fclose(bitstream);
}