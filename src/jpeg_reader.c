#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


/*THIS C SCRIPT WILL EXTRACT THE JPEG HEADER WITH ALL SECTIONS FROM THE JPEG_ASCII.TXT FILE*/

/*WE'LL USE STRUCTURES HERE TO DEFINE THE DIFFERENT SECTIONS AND THE INFORMATION
WE NEED FOR DECODING*/


struct APP0 {
    char image_type[5];
    uint16_t section_length;

};

struct DQT {
    uint16_t section_length;
    uint8_t i_q;
    char* quantification_values;

};

struct SOF {
    uint16_t section_length;
    uint16_t height;
    uint16_t width;
    uint8_t components_number;
    uint32_t* i_c;
    uint16_t* sampling_horizontal;
    uint16_t* sampling_vertical;
    uint32_t* quantification_table_i_q;

};

struct DHT{
    uint16_t section_length;
    char huffman_information[2];
    char symbols_number[17];
    char symbols[257];

};

struct SOS{
    uint16_t section_length;
    uint8_t components_number;
    uint64_t* i_c;
    uint32_t* i_h_DC;
    uint32_t* i_h_AC;

};

struct APP0* extract_app0(FILE* file){
    /*EXTRACTS THE APP0 SECTION FROM THE FILE*/
    unsigned char length[4];
    /*TAKES THE FIRST TWO BYTES*/
    fread(length,sizeof(unsigned char),4,file);
    /*INITIALIZE APP0 STRUCTURE*/
    struct APP0* app0=malloc(sizeof(struct APP0));
    app0->section_length=(uint16_t) strtoul(length,NULL,16);
    printf("%u\n",app0->section_length);
    unsigned char type[10];
    /* TAKE THE NEXT 5 BYTES */
    fread(type,sizeof(unsigned char),10,file);
    printf("%s\n",type);
    /*CONVERT THE HEXADECIMAL STRING TO ASCII STRING 
    AND VERIFY THAT IT IS JFIF*/
    unsigned char* converted=convert_hex_to_str(type);
    app0->image_type = malloc(strlen(converted) + 1);
    strcpy(app0->image_type,converted);
    
    if (strcmp(converted,"JFIF")!=0){
        fprintf(stderr,"Error : encapsulation format is not JFIF\n");
        exit(EXIT_FAILURE);
    }
    free(converted);
    printf("%s\n",app0->image_type);
    /*RETURN APP0 STRUCTURE THAT CONTAINS SECTION LENGTH AND IMAGE_ENCAPSULATION JFIF*/
    return app0;
}


void extract_header(void){
    FILE* image=fopen("jpeg_ascii.txt","r");
    unsigned char caracters[3];
    while (fread(caracters,sizeof(unsigned char),2,image)==2){
        if (strcmp(caracters,"FF")==0){
            fgets(caracters,2,image);
            if (strcmp(caracters,"D8")==0){
                continue;}
            
            if (strcmp(caracters,"E0")==0){
                struct APP0 app0=extract_app0(image);
            } 
        
            
            


        }
    }

}








