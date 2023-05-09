#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

struct APP0 {
    unsigned char* image_type;
    uint16_t section_length;

};

struct DQTs {
    struct DQT* all_dqt;
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
    uint8_t* i_c;
    uint8_t* sampling_horizontal;
    uint8_t* sampling_vertical;
    uint8_t* quantification_table_i_q;

};

struct DHTs {
    struct DHT* all_dht;
};

struct DHT{
    uint16_t section_length;
    unsigned char huffman_information[2];
    uint8_t symbols_number[16];
    unsigned char* symbols;

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
    for (int i = 0; i < len_string; i += 2){
        char str_pair[3] = { string[i], string[i+1], '\0' };
        uint8_t ascii_value = strtoul(str_pair, NULL, 16);
        string_ascii[i/2] = (char)ascii_value;
    }
    string_ascii[len_string/2] = '\0';


    return string_ascii;
}




struct APP0* EXTRACT_APP0(FILE* file){
    
    /*EXTRACTS THE APP0 SECTION FROM THE FILE*/
    unsigned char length[4];
    
    /*TAKES THE FIRST TWO BYTES*/
    fread(length,sizeof(unsigned char),4,file);
    
    /*INITIALIZE APP0 STRUCTURE*/
    struct APP0* app0=malloc(sizeof(struct APP0));
    app0->section_length=(uint16_t) strtoul(length,NULL,16);
    /*printf("%u\n",app0->section_length);*/
    unsigned char type[10];
    
    /* TAKE THE NEXT 5 BYTES */
    fread(type,sizeof(unsigned char),10,file);
    /*printf("%s\n",type);*/
    
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
    /*printf("%s\n",app0->image_type);*/
    
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
    dqt->i_q=(uint8_t) strtoul(i_q,NULL,16);
    
    /*INITIALIZE DQT QUATIFICATION TABLE */
    int pos=ftell(file);
    /*printf("%i\n",pos);*/
    int index_tab=0;
    for (int i=pos;i<=pos+(dqt->section_length);i++){
        fread(&(dqt->quantification_values)[index_tab],sizeof(unsigned char),1,file);
        /*printf("%c",dqt->quantification_values[index_tab]);*/
        index_tab+=1;
    }
    
    /*RETURN DQT STRUCTURE*/
    return dqt;
    
}



struct SOF* EXTRACT_SOF(FILE* file){
    unsigned char* length=malloc(4*sizeof(unsigned char));
    memset(length,0,4);
    /*TAKES THE FIRST TWO BYTES*/
    fread(length,sizeof(unsigned char),4,file);
    
    /*INITIALIZE SOF STRUCTURE*/
    struct SOF* sof= malloc(sizeof(struct SOF));
    sof->section_length=(uint16_t) strtoul(length,NULL,16);
    free(length);
    /*printf("%u\n",sof->section_length);*/
    unsigned char* ignore=malloc(2*sizeof(unsigned char));
    memset(ignore,0,2);
    fread(ignore,sizeof(unsigned char),2,file);
    free(ignore);
   /*INITIALIZE HEIGHT*/
    unsigned char* height=malloc(4*sizeof(unsigned char));
    memset(height,0,4);
    fread(height,sizeof(unsigned char),4,file);
    sof->height=(uint16_t) strtoul(height,NULL,16);
    free(height);
    /*printf("%u\n",sof->height);*/

    /*INITIALIZE WIDTH*/
    unsigned char* width=malloc(4*sizeof(unsigned char));
    memset(width,0,4);
    fread(width,sizeof(unsigned char),4,file);
    sof->width=(uint16_t) strtoul(width,NULL,16);
    free(width);
    /*printf("%u\n",sof->width);*/

    /*INITIALIZE COMPONENTS NUMBER*/
    unsigned char compnumber[2];
    memset(compnumber,0,2);
    fread(compnumber,sizeof(char),2,file);
    sof->components_number=(uint8_t) strtoul(compnumber,NULL,16);
    /*printf("%u\n",sof->components_number);*/
    
    /*INITIALIZE I_C*/
    sof->i_c=malloc(sof->components_number*sizeof(uint8_t));
    sof->sampling_horizontal=malloc(sof->components_number*sizeof(uint8_t));
    sof->sampling_vertical=malloc(sof->components_number*sizeof(uint8_t));
    sof->quantification_table_i_q=malloc(sof->components_number*sizeof(uint8_t));
    for (int i=0;i<sof->components_number;i++){
        fread(&(sof->i_c)[i],sizeof(uint8_t),2,file);
        fread(&(sof->sampling_horizontal)[i],sizeof(uint8_t),1,file);
        fread(&(sof->sampling_vertical)[i],sizeof(uint8_t),1,file);
        fread(&(sof->quantification_table_i_q)[i],sizeof(uint8_t),2,file);


    }
    
    return sof;
}



struct DHT* EXTRACT_DHT(FILE* file){
    unsigned char length[4];
    /*TAKES THE FIRST TWO BYTES*/
    fread(length,sizeof(unsigned char),4,file);
    
    /*INITIALIZE DHT STRUCTURE*/
    struct DHT* dht= malloc(sizeof(struct DHT));
    dht->section_length=(uint16_t) strtoul(length,NULL,16);
    printf("%u\n",dht->section_length);

    /*INITIALIZE HUFFMAN INFORMATION*/
    fread(&(dht->huffman_information)[0],sizeof(unsigned char),1,file);
    fread(&(dht->huffman_information)[1],sizeof(unsigned char),1,file); 
    /*printf("%c\n",dht->huffman_information[0]);
    /*printf("%c\n",dht->huffman_information[1]);

    /*INITIALIZE SYMBOLS NUMBER*/
    for (int i=0;i<32;i+=2){
        unsigned char* number_str=malloc(2*sizeof(unsigned char));
        fread(number_str,sizeof(unsigned char),2,file);
        /*printf("%s\n",number_str);*/
        dht->symbols_number[i/2]= (uint8_t) strtoul(number_str,NULL,16);
        free(number_str);
        /*printf("%u\n",dht->symbols_number[i/2]);
    }

    /*INITIALIZE SYMBOLS*/
    uint8_t length_symbols_table=dht->section_length-19;
    printf("%u",length_symbols_table);
    dht->symbols=malloc(length_symbols_table*sizeof(unsigned char));
    for (int i=0;i<length_symbols_table;i++) {
        fread(&(dht->symbols)[i],sizeof(unsigned char),2,file);
        printf("%c\n",dht->symbols[i]); 
        
    }

    return dht;
}



void extract_header(void){
    
    FILE* image=fopen("jpeg_ascii.txt","r");
    unsigned char caracters[2];
    struct DHTs* dhts=malloc(sizeof(struct DHT));
    struct DQTs* dqts=malloc(sizeof(struct DQT));
    while (fread(caracters,sizeof(unsigned char),2,image)==2){ /*READ 2 BY 2 BYTES IN THE .TXT FILE*/
        if (strcmp(caracters,"FF")==0){ /* IF WE HAVE A BEGINNING OF A MARKER*/
            fread(caracters,sizeof(unsigned char),2,image); /* ADVANCE BY 2 BYTES*/
            if (strcmp(caracters,"D8")==0){ /* IF MARKER IS SOI, CONTINUE */
                continue;
                }
            
            if (strcmp(caracters,"E0")==0){ /* ELSE IF MARKER APP0, CALL EXTRACT_APP0 */
                struct APP0* app0=EXTRACT_APP0(image); /*CALL EXTRACT_APP0 FUNCTION AND RETURN THE APP0 STRUCTURE */
                /*printf("%s\n",app0->image_type);*/    
                /*printf("%u\n",app0->section_length);*/
                int position=ftell(image); /*TAKE THE CURRENT POSITION */
                fseek(image,position+(app0->section_length*2)-14,SEEK_SET); /*ADVANCE INTO THE NEXT FF*/
                
                
                
                
            }

            if (strcmp(caracters,"DB")==0){
                struct DQT* dqt=EXTRACT_DQT(image);
                /*printf("%u\n",dqt->i_q);
                printf("%u\n",dqt->section_length);
                for (int i=0;i<64;i++){
                    printf("%c",dqt->quantification_values[i]);
                }*/
            }
               

            
            if (strcmp(caracters,"C0")==0){
                struct SOF* sof=EXTRACT_SOF(image);
                /*for (int i=0;i<sof->components_number;i++){
                    printf("%c\n",sof->i_c[i]);
                    printf("%c\n",sof->sampling_horizontal[i]);
                    printf("%c\n",sof->sampling_vertical[i]);
                    printf("%c\n",sof->quantification_table_i_q[i]);
                }*/
                
            }

            if (strcmp(caracters,"C4")==0){
                struct DHT* dht=EXTRACT_DHT(image);
                
            }
        
            
            


        }
    }
    fclose(image);
}


void hexbump(char* file){
    /*This function will create a "jpeg_ascii.txt" file in our /src directory*/


    /* Open the file and the hexbumped file <3 */
    FILE* jpeg_image= fopen(file,"r");
    FILE* jpeg_ascii= fopen("jpeg_ascii.txt","w");
    
    /*  Declare byte to read (unsigned to prevent negative values)  */
    unsigned char byte_to_read;
    
    /*  Print jpeg content two bytes after the others  */
    while (fread(&byte_to_read,sizeof(byte_to_read),1,jpeg_image)==1){
        fprintf(jpeg_ascii,"%02X",byte_to_read);
    }

    /* Close the input image file */
    fclose(jpeg_image);
    fclose(jpeg_ascii);

    
}


int main(int argc,char** argv){
    
    /* If there is no image.jpeg input : USAGE ERROR*/
    if (argc!=2){
        fprintf(stderr,"Usage: %s fichier.jpeg\n",argv[0]);
        return EXIT_FAILURE;
    }

    /* Check if the image is in jpeg or jpg format */
    size_t len = strlen(argv[1]);
    if (len < 5) {
        /* the string is too short to end with ".jpeg" or ".jpg" */
        return EXIT_FAILURE;
    }

    /* Return Error in this case */
    const char* suffix1 = argv[1] + len - 4;
    const char* suffix2 = argv[1] + len - 5;
    if (!(strcmp(suffix2, ".jpeg") == 0 || strcmp(suffix1, ".jpg") == 0)){
        fprintf(stderr,"Wrong Format: PLEASE INSERT JPEG IMAGE\n ");
        return EXIT_FAILURE;
    }

    /*Convert image file to hexBump file in order to extract header*/
    hexbump(argv[1]); /*This will create a "jpeg_ascii.txt" file that contains the hexbumped version*/
    extract_header();

    




    

    

    
    
 
    
    



    



    return EXIT_SUCCESS;
}