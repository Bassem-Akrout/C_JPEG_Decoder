#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


/*THIS C SCRIPT WILL EXTRACT THE JPEG HEADER WITH ALL SECTIONS FROM THE JPEG_ASCII.TXT FILE*/

/*WE'LL USE STRUCTURES HERE TO DEFINE THE DIFFERENT SECTIONS AND THE INFORMATION
WE NEED FOR DECODING*/


struct APP0 {
    unsigned char* image_type;
    uint16_t section_length;

};

struct DQT {
    uint16_t section_length;
    uint8_t i_q;
    unsigned char quantification_values[64];

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



char* convert_hex_to_str(char* string){
    /* THIS FUNCTION CONVERTS A HEXADECIMAL STRING TO ASCCI_STRING*/
    int len_string = strlen(string);
    char* string_ascii = (char*)malloc((len_string/2) + 1);

    /* Convert 2 by 2 (byte by byte) into ascii caracter*/
    for (int i = 0; i < len_string; i += 2) {
        char str_pair[3] = { string[i], string[i+1], '\0' };
        uint8_t ascii_value = strtoul(str_pair, NULL, 16);
        string_ascii[i/2] = (char)ascii_value;
    }
    string_ascii[len_string/2] = '\0';


    return string_ascii;
}


struct APP0* EXTRACTAPP0(FILE* file){
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


struct DQT* EXTRACT_DQT(FILE* file){
    
    /*EXTRACTS THE DQT SECTION FROM THE FILE*/
    unsigned char length[4];
    
    /*TAKES THE FIRST TWO BYTES*/
    fread(length,sizeof(unsigned char),4,file);
    
    /*INITIALIZE DQT STRUCTURE*/
    struct DQT* dqt=malloc(sizeof(struct DQT));
    dqt->section_length=(uint16_t) strtoul(length,NULL,16);
    /*printf("%u\n",dqt->section_length);*/
    
    /*INITIALIZE QUANTIFICATION INDEX*/
    unsigned char* i_q;
    fread(i_q,sizeof(unsigned char),1,file);
    fread(i_q,sizeof(unsigned char),1,file);
    dqt->i_q=(uint8_t) strtoul(i_q,NULL,8);
    
    /*INITIALIZE DQT QUATIFICATION TABLE */
    /*unsigned char* dqt_table=calloc(64,sizeof(unsigned char));
    */int pos=ftell(file);
    /*printf("%i\n",pos);*/
    int index_tab=0;
    for (int i=pos;i<=pos+(dqt->section_length);i++){
        fread(&(dqt->quantification_values)[index_tab],sizeof(unsigned char),1,file);
        /*printf("%c",dqt->quantification_values[index_tab]);*/
        index_tab+=1;
    
    }
    

    /*memcpy(dqt->quantification_values, dqt_table, 32);*/
    /*RETURN DQT STRUCTURE*/
    return dqt;
    
}


struct SOF* EXTRACT_SOF(FILE* file){
    unsigned char length[4];
    /*TAKES THE FIRST TWO BYTES*/
    fread(length,sizeof(unsigned char),4,file);
    
    /*INITIALIZE DQT STRUCTURE*/
    struct SOF* sof=malloc(sizeof(struct SOF));
    sof->section_length=(uint16_t) strtoul(length,NULL,16);
    printf("%u",sof->section_length);

    return sof;
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
                struct APP0* app0=EXTRACTAPP0(image);
                int position=ftell(image); /*TAKE THE CURRENT POSITION */
                fseek(image,position+(app0->section_length*2)-14,SEEK_SET); /*ADVANCE INTO THE NEXT FF*/
                
            }

            if (strcmp(caracters,"DB")==0){
                struct DQT* dqt=EXTRACT_DQT(image);
            } 

            if (strcmp(caracters,"C0")==0){
                struct SOF* sof=EXTRACT_SOF(image);
            }


        
            
            


        }
    }

}








