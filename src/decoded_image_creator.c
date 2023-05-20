#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/jpeg_reader.h"
#include <math.h>


char * create_name_file(char * jpeg_name, uint8_t nb_components){
    uint8_t length =strlen(jpeg_name);
    char * new_name =malloc(length*sizeof(char));
    strcpy(new_name,jpeg_name);
    uint8_t pos_change_type=length-4;
    if (new_name[pos_change_type]!='.'){
        pos_change_type-=1;
    }
    new_name[pos_change_type+1]='p';
    new_name[pos_change_type+2]='p';
    if(nb_components==1) {
        new_name[pos_change_type+2]='g';
    }
    new_name[pos_change_type+3]='m';
    new_name[pos_change_type+4]='\0';
    return new_name;
}

void blackandwhite(uint8_t** MCUs_RGB,struct SOF* sof,char* jpeg_name){
    
    // INTITIALIZE PGM IMAGE
    char *new_file = create_name_file(jpeg_name,1);
    FILE *new_image = fopen(new_file,"wb");
    char* format="P5";
    char* rgb="255";
    uint32_t MCU_hori_number =(1+(uint32_t)ceil(sof->width/(sof->sampling_horizontal[0]*8)));
    uint32_t MCU_vertical_number=(1+(uint32_t)ceil(sof->height/(sof->sampling_vertical[0]*8)));
    fprintf(new_image,"%s\n",format);
    fprintf(new_image,"%u %u\n",(sof->width),(sof->height));
    fprintf(new_image,"%s\n",rgb);
    uint8_t jmax=8;
    uint8_t kmax=8;

    // WRITE CONTENT IN PGM IMAGE
    if((ceil(sof->width/(sof->sampling_horizontal[0]*8)))!=((double)(sof->width)/(double)((sof->sampling_horizontal[0]*8)))){
        for (uint32_t l=0;l<MCU_vertical_number;l++){
        if (l==MCU_vertical_number-1){jmax=sof->height%8;}
        else{jmax=8;}
            for (int j = 0; j < jmax; j++)
                for (uint32_t i = 0; i < MCU_hori_number ;i++){
                if (i==MCU_hori_number-1){kmax=sof->width%8;}
                else{kmax=8;}
                    for (int k = 0; k < kmax;k++)
                        fwrite(&(MCUs_RGB[i+l*(MCU_hori_number)][j*8+k]),sizeof(uint8_t),1,new_image);}
            }
        }
    else{
        for (uint32_t l=0;l<MCU_vertical_number-1;l++)
            for (int j = 0; j < 8; j++)
                for (uint32_t i = 0; i < MCU_hori_number -1;i++)
                    for (int k = 0; k < 8;k++)
                        fwrite(&(MCUs_RGB[i+l*(MCU_hori_number-1)][j*8+k]),sizeof(uint8_t),1,new_image);
    }
    
                        
    fclose(new_image);
    free(new_file);


}


void rainbow(uint8_t*** MCUs_RGB,struct SOF* sof,char* jpeg_name){
    char *new_file = create_name_file(jpeg_name,sof->components_number);
    FILE *new_image = fopen(new_file,"wb");
    char* format="P6";
    char* rgb="255";
    fprintf(new_image,"%s\n",format);
    fprintf(new_image,"%u %u\n",sof->width,sof->height);
    fprintf(new_image,"%s\n",rgb);
    

// PARTIE ENTIÈRE NOMBRE DES MCUS : sof->height/sof->sampling_vertical[0]*8 .. 


    for (uint32_t l=0;l<floor(sof->height/(sof->sampling_vertical[0]*8));l++)
        for (int j = 0; j < sof->sampling_vertical[0]*8; j++)
            for (uint32_t i = 0; i < floor(sof->width/(sof->sampling_horizontal[0]*8));i++){
                for (int m=0;m<sof->sampling_horizontal[0];m++){
		            for (int k = m*sof->sampling_vertical[0]*64; k < m*sof->sampling_vertical[0]*64+sof->sampling_vertical[0]*8;k++)
                        {
                        fwrite(&(MCUs_RGB[i+l*(uint32_t)floor(sof->width/(sof->sampling_horizontal[0]*8))][j*8+k][0]),sizeof(uint8_t),1,new_image);
                        fwrite(&(MCUs_RGB[i+l*(uint32_t)sof->width/(sof->sampling_horizontal[0]*8)][j*8+k][1]),sizeof(uint8_t),1,new_image);
                        fwrite(&(MCUs_RGB[i+l*(uint32_t)sof->width/(sof->sampling_horizontal[0]*8)][j*8+k][2]),sizeof(uint8_t),1,new_image);
                    }

                }
}
}

