#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>



struct HEADER{
    struct APP0* app0;
    struct SOF* sof;
    struct SOS* sos;
    struct DQT* dqt_lista[4];
    struct DHT* dht_lista[4];
    uint8_t* dqt_countra;
    uint8_t* dht_countra;

};



struct APP0 {
    unsigned char* image_type;
    uint16_t section_length;

};



struct DQT {
    uint16_t section_length;
    uint8_t i_q;
    unsigned char* quantification_values;

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


struct DHT{
    uint16_t section_length;
    unsigned char huffman_information[2];
    uint8_t symbols_number[16];
    unsigned char* symbols;

};

struct SOS{
    uint16_t section_length;
    uint8_t components_number;
    uint8_t* i_c;
    uint8_t* i_h_DC;
    uint8_t* i_h_AC;

};


unsigned char* hex_to_bin(unsigned char* hex) {
    unsigned char* bin = malloc(5 * sizeof(unsigned char));

    switch (*hex) {
        case '0':
            strcpy(bin, "0000");
            break;
        case '1':
            strcpy(bin, "0001");
            break;
        case '2':
            strcpy(bin, "0010");
            break;
        case '3':
            strcpy(bin, "0011");
            break;
        case '4':
            strcpy(bin, "0100");
            break;
        case '5':
            strcpy(bin, "0101");
            break;
        case '6':
            strcpy(bin, "0110");
            break;
        case '7':
            strcpy(bin, "0111");
            break;
        case '8':
            strcpy(bin, "1000");
            break;
        case '9':
            strcpy(bin, "1001");
            break;
        case 'A':
        case 'a':
            strcpy(bin, "1010");
            break;
        case 'B':
        case 'b':
            strcpy(bin, "1011");
            break;
        case 'C':
        case 'c':
            strcpy(bin, "1100");
            break;
        case 'D':
        case 'd':
            strcpy(bin, "1101");
            break;
        case 'E':
        case 'e':
            strcpy(bin, "1110");
            break;
        case 'F':
        case 'f':
            strcpy(bin, "1111");
            break;
        default:
            fprintf(stderr, "Erreur : caractère hexadécimal non valide\n");
            free(bin);
            return NULL;
    }

    return bin;
}      


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
    unsigned char* i_q=calloc(1,sizeof(unsigned char));
    fread(i_q,sizeof(unsigned char),1,file);
    fread(i_q,sizeof(unsigned char),1,file);
    dqt->i_q=(uint8_t) strtoul(i_q,NULL,16);
    free(i_q);
    
    /*INITIALIZE DQT QUATIFICATION TABLE */
    dqt->quantification_values=calloc(64,2*sizeof(unsigned char));
    /*printf("%i\n",pos);*/
    for (int i=0;i<128;i++){
        fread(&(dqt->quantification_values)[i],sizeof(unsigned char),1,file);
        /*printf("qt: %s\n",&(dqt->quantification_values)[i]);*/
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
    fread(compnumber,sizeof(unsigned char),2,file);
    sof->components_number=(uint8_t) strtoul(compnumber,NULL,16);
    /*printf("%u\n",sof->components_number);*/
    
    /*INITIALIZE I_C*/
    sof->i_c=malloc(sof->components_number*sizeof(uint8_t));
    sof->sampling_horizontal=malloc(sof->components_number*sizeof(uint8_t));
    sof->sampling_vertical=malloc(sof->components_number*sizeof(uint8_t));
    sof->quantification_table_i_q=malloc(sof->components_number*sizeof(uint8_t));
    for (int i=0;i<sof->components_number;i++){
        unsigned char* ic_loc=calloc(2,sizeof(unsigned char));
        fread(ic_loc,sizeof(unsigned char),2,file);
        (sof->i_c)[i]=(uint8_t) strtoul(ic_loc,NULL,16);
        /*printf("i_c: %u\n",(sof->i_c)[i]);*/
        unsigned char* samp_loc=calloc(1,sizeof(unsigned char));
        fread(samp_loc,sizeof(unsigned char),1,file);
        (sof->sampling_horizontal)[i]=(uint8_t) strtoul(samp_loc,NULL,16);
        /*printf("samp horz: %u\n",(sof->sampling_horizontal)[i]);*/
        fread(samp_loc,sizeof(unsigned char),1,file);
        (sof->sampling_vertical)[i]=(uint8_t) strtoul(samp_loc,NULL,16);
        /*printf("samp vertical: %u\n",(sof->sampling_vertical)[i]);*/
        fread(ic_loc,sizeof(unsigned char),2,file);
        (sof->quantification_table_i_q)[i]=(uint8_t) strtoul(ic_loc,NULL,16);
        /*printf("iq: %u\n",(sof->quantification_table_i_q)[i]);*/
        free(ic_loc);
        free(samp_loc);



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
    /*printf("LENGTH: %u\n",dht->section_length);*/

    /*INITIALIZE HUFFMAN INFORMATION*/
    fread(&(dht->huffman_information)[0],sizeof(unsigned char),1,file);
    fread(&(dht->huffman_information)[1],sizeof(unsigned char),1,file); 
    /*printf("INFO1: %c\n",dht->huffman_information[0]);*/
    /*printf("INFO2: %c\n",dht->huffman_information[1]);*/

    /*INITIALIZE SYMBOLS NUMBER*/
    for (int i=0;i<32;i+=2){
        unsigned char* number_str=calloc(2,sizeof(unsigned char));
        fread(number_str,sizeof(unsigned char),2,file);
        /*printf("%s\n",number_str);*/
        dht->symbols_number[i/2]= (uint8_t) strtoul(number_str,NULL,16);
        free(number_str);
        /*printf("NOMBRE DE SYMBOLES: %u\n",dht->symbols_number[i/2]);*/
    }
    /*printf("\n");*/

    /*INITIALIZE SYMBOLS*/
    uint8_t length_symbols_table=dht->section_length-19;
    /*printf("LENGTH: %u",length_symbols_table);*/
    dht->symbols=calloc(2*length_symbols_table,sizeof(unsigned char));
    for (int i=0;i<2*length_symbols_table ;i++) {
        fread(&(dht->symbols)[i],sizeof(unsigned char),1,file);
    }
    return dht;
}


struct SOS* EXTRACT_SOS(FILE* file){
    unsigned char length[4];
    /*TAKES THE FIRST TWO BYTES*/
    fread(length,sizeof(unsigned char),4,file);
    
    /*INITIALIZE SOS STRUCTURE*/
    struct SOS* sos= malloc(sizeof(struct SOS));
    sos->section_length=(uint16_t) strtoul(length,NULL,16);
    /*printf("LENGTH: %u\n",sos->section_length);*/

    /*INITIALIZE COMPONENT NUMBER*/
    unsigned char* compnumber=calloc(2,sizeof(unsigned char));
    fread(compnumber,sizeof(char),2,file);
    sos->components_number=(uint8_t) strtoul(compnumber,NULL,16);
    /*printf("N=: %u\n",sos->components_number);*/
    /*INITIALIZE I_C , I_H_AC, I_H_DC */
    sos->i_c=malloc(sos->components_number*sizeof(uint8_t));
    sos->i_h_AC=malloc(sos ->components_number*sizeof(uint8_t));
    sos->i_h_DC=malloc(sos ->components_number*sizeof(uint8_t));
    for (int i=0;i<sos ->components_number;i++){
        unsigned char* ic_loc=calloc(2,sizeof(unsigned char));
        fread(ic_loc,sizeof(unsigned char),2,file);
        (sos ->i_c)[i]=(uint8_t) strtoul(ic_loc,NULL,16);
        /*printf("i_c: %u\n",(sos ->i_c)[i]);*/
        unsigned char* i_h_loc=calloc(1,sizeof(unsigned char));
        fread(i_h_loc,sizeof(unsigned char),1,file);
        (sos ->i_h_DC)[i]=(uint8_t) strtoul(i_h_loc,NULL,16);
        /*printf("DC: %u\n",(sos->i_h_DC)[i]);*/
        fread(i_h_loc,sizeof(unsigned char),1,file);
        (sos->i_h_AC)[i]=(uint8_t) strtoul(i_h_loc,NULL,16);
        /*printf("AC: %u\n",(sos ->i_h_AC)[i]);*/
        free(i_h_loc);
        free(ic_loc);
    }
    /*FSEEK TO ADVANCE IN BITSTREAM*/
    int pos=ftell(file);
    fseek(file,pos+6,SEEK_SET);
}

void extract_header(struct HEADER* header){
    
    FILE* image=fopen("jpeg_ascii.txt","r");
    unsigned char caracters[2];
    struct APP0* app0;
    struct DQT* dqt;
    struct DHT* dht;
    struct SOF* sof;
    struct SOS* sos;
    uint8_t dqt_counter=0;
    header->dqt_countra=&dqt_counter;
    uint8_t dht_counter=0;
    header->dht_countra=&dht_counter;
    while (fread(caracters,sizeof(unsigned char),2,image)==2){ /*READ 2 BY 2 BYTES IN THE .TXT FILE*/
        if (strcmp(caracters,"FF")==0){ /* IF WE HAVE A BEGINNING OF A MARKER*/
            fread(caracters,sizeof(unsigned char),2,image); /* ADVANCE BY 2 BYTES*/
            if (strcmp(caracters,"D8")==0){ /* IF MARKER IS SOI, CONTINUE */
                continue;
                }
            
            if (strcmp(caracters,"E0")==0){ /* ELSE IF MARKER APP0, CALL EXTRACT_APP0 */
                app0=EXTRACT_APP0(image); 
                header->app0=app0;          /*CALL EXTRACT_APP0 FUNCTION AND RETURN THE APP0 STRUCTURE */
                /*printf("IMAGE TYPE: %s\n",app0->image_type);    
                printf("APP0 LENGTH: %u\n",app0->section_length);*/
                int position=ftell(image); /*TAKE THE CURRENT POSITION */
                fseek(image,position+(app0->section_length*2)-14,SEEK_SET); /*ADVANCE INTO THE NEXT FF*/
                
                
                
                
            }

            if (strcmp(caracters,"DB")==0){
            header->dqt_lista[dqt_counter]=EXTRACT_DQT(image);
            /*printf("DQT IQ: %u\n",dqt->i_q);
            printf("LENGTH DQT: %u\n",dqt->section_length);*/
            /*for (int i=0;i<128;i++){
            printf("qt: %s\n",dqt_list[dqt_counter]->quantification_values);
            printf("\n");
            }*/
            /*printf("dqt_counter: %d qt: %s \n",dqt_counter,dqt_list[dqt_counter]->quantification_values);*/
            dqt_counter+=1;
            header->dqt_countra=&dqt_counter;
            
            }
            
               

            
            if (strcmp(caracters,"C0")==0){
                sof=EXTRACT_SOF(image);
                /*for (int i=0;i<sof->components_number;i++){
                    printf("%c\n",sof->i_c[i]);
                    printf("%c\n",sof->sampling_horizontal[i]);
                    printf("%c\n",sof->sampling_vertical[i]);
                    printf("%c\n",sof->quantification_table_i_q[i]);
                }*/
                header->sof=sof;
                
            }

            if (strcmp(caracters,"C4")==0){
            header->dht_lista[dht_counter]=EXTRACT_DHT(image);

            /*printf("dht_counter: %d NUMBERS: ",dht_counter);
            for (int i=0;i<16;i++){
            printf("%u ",dht_list[dht_counter]->symbols_number[i]);
            } 
            printf("\n");
            printf("dht_counter: %d SYMBOLS: ",dht_counter);
            for (int i=0;i<2*(dht_list[dht_counter]->section_length-19);i++){
            printf("%c ",dht_list[dht_counter]->symbols[i]);
            }
            printf("\n");*/
            dht_counter+=1;
            header->dht_countra=&dht_counter;
            printf("dht evolutif %i \n",*(header->dht_countra));
            
            }
            

            if (strcmp(caracters,"DA")==0){
                sos=EXTRACT_SOS(image);
                /*for (int i=0;i<sos->components_number;i++){
                    printf("IC: %u\n",sos->i_c[i]);
                    printf("I_H_AC: %u\n",sos->i_h_AC[i]);
                    printf("I_H_DC: %u\n",sos->i_h_DC[i]);
                    
                }*/
                header->sos=sos;
                FILE* bitstream=fopen("bitstream.txt","wb");
                unsigned char byte_to_read[1];
                /*  Print bitstream two bytes after the others  */
                while (fread(byte_to_read,sizeof(byte_to_read),1,image)==1){
                    unsigned char* strbin=hex_to_bin(byte_to_read);
                    fprintf(bitstream,"%s",strbin);
                
                }
                fseek(bitstream,-16,SEEK_END);
                ftruncate(fileno(bitstream),ftell(bitstream));
                fclose(bitstream);
            printf("dht evolutif %i \n",*(header->dht_countra));


            }

            


        }
    }
    fclose(image);
    
}



void hexbump(char* file){
    /*This function will create a "jpeg_ascii.txt" file in our /src directory*/


    /* Open the file and the hexbumped file <3 */
    FILE* jpeg_image= fopen(file,"r");
    FILE* jpeg_ascii= fopen("jpeg_ascii.txt","r+w");
    
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
    struct HEADER* header=calloc(1,sizeof(struct HEADER));
    extract_header(header);
    
    printf("%s\n",header->app0->image_type);
    printf("%u\n",header->sof->components_number);

    printf("teset main\n");
   
    printf("dht non evolutif %i \n",*(header->dht_countra));

    printf("qt: %s\n",header->dqt_lista[*(header->dqt_countra)]->quantification_values);
    printf("\n");
    




    

    

    
    
 
    
    



    



    return EXIT_SUCCESS;
}