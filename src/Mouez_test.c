#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h> /* for the function ntohs() */


struct HEADER{
    struct APP0* app0;
    struct SOF* sof;
    struct SOS* sos;
    struct DQTs* dqts;
    struct DHT* dht;
};

struct DQTs{
    struct DQT** dqt_table;
    int dqt_counter;
};

struct DHTs{
    struct DHT* dht;
};

struct APP0 {
    uint16_t section_length;
    char* image_type;
};

struct DQT {
    uint16_t section_length;
    uint8_t i_q;
    uint8_t* quantification_values;
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

struct APP0* EXTRACT_APP0(FILE* file){
    printf("APP0 info: \n");
    struct APP0* app0 = malloc(sizeof(struct APP0));
    fread(&(app0->section_length), sizeof(uint16_t), 1, file);
    app0->section_length = ntohs(app0->section_length);
    printf("%u\n", app0->section_length);
    app0->image_type = malloc(sizeof(char)*5);
    fread(app0->image_type, sizeof(char), 5, file);
    printf("%s\n",app0->image_type);
    if (strncmp(app0->image_type, "JFIF", 4) != 0) {
        printf("Warning: image type is not JFIF\n");
        exit(EXIT_FAILURE);
    }
    return app0;
}

struct DQT* EXTRACT_DQT(FILE* file){
    printf("DQT info: \n");
    struct DQT* dqt = malloc(sizeof(struct DQT));
    fread(&(dqt->section_length), sizeof(uint16_t), 1, file);
    dqt->section_length = ntohs(dqt->section_length);
    printf("%u\n", dqt->section_length);
    fread(&(dqt->i_q),sizeof(uint8_t),1,file);
    if(dqt->i_q>0x01){
        dqt->i_q=dqt->i_q-16;
    }
    printf("iq:%u\n",dqt->i_q);
    dqt->quantification_values=calloc(64,sizeof(uint8_t));
    for (int i=0;i<64;i++){
        uint8_t quant_val;
        fread(&quant_val,sizeof(uint8_t),1,file);
        dqt->quantification_values[i] = quant_val;
        printf("%02X ",dqt->quantification_values[i]);
    }
    printf("\n");
    return dqt;
}
struct SOF* EXTRACT_SOF(FILE* file){
    printf("SOF info: \n");
    /*INITIALIZE SOF STRUCTURE*/
    struct SOF* sof= malloc(sizeof(struct SOF));
    /*INITIALIZE SECTION LENGTH*/
    fread(&(sof->section_length), sizeof(uint16_t), 1, file);
    sof->section_length = ntohs(sof->section_length);
    printf("%u\n", sof->section_length);
    /*ADVANCE*/
    int pos=ftell(file);
    fseek(file,pos+1,SEEK_SET);
    /*INITIALIZE HEIGHT*/
    fread(&(sof->height),sizeof(uint16_t),1,file);
    sof->height = ntohs(sof->height);
    printf("%u\n", sof->height);
    /*INITIALIZE WIDTH*/
    fread(&(sof->width),sizeof(uint16_t),1,file);
    sof->width = ntohs(sof->width);
    printf("%u\n", sof->width);
    /*INITIALIZE COMPONENTS NUMBER*/
    fread(&(sof->components_number),sizeof(uint8_t),1,file);
    printf("N:%u\n", sof->components_number);
    /*INITIALIZE I_C*/
    sof->i_c=malloc(sof->components_number*sizeof(uint8_t));
    sof->sampling_horizontal=malloc(sof->components_number*sizeof(uint8_t));
    sof->sampling_vertical=malloc(sof->components_number*sizeof(uint8_t));
    sof->quantification_table_i_q=malloc(sof->components_number*sizeof(uint8_t));
     for (int i=0;i<sof->components_number;i++){
        fread(&(sof->i_c)[i],sizeof(uint8_t),1,file);
        fread(&(sof->sampling_horizontal)[i],sizeof(uint8_t),1,file);
        fread(&(sof->quantification_table_i_q)[i],sizeof(uint8_t),1,file);
        sof->sampling_vertical[i]=(sof->sampling_horizontal[i])%16;
        sof->sampling_horizontal[i]=(sof->sampling_horizontal[i])/16;
        printf("i_c: %u\n",(sof->i_c)[i]);
        printf("samp horiz: %u\n",(sof->sampling_horizontal)[i]);
        printf("samp vert: %u\n",(sof->sampling_vertical)[i]);
        printf("iq: %u\n",(sof->quantification_table_i_q)[i]);
     }
     return sof;
}

struct DHT* EXTRACT_DHT(FILE* file){
    printf("DHT info: \n");
    /*INITIALIZE DHT STRUCTURE*/
    struct DHT* dht= malloc(sizeof(struct DHT));
    /*INITIALIZE SECTION LENGTH*/
    fread(&(dht->section_length), sizeof(uint16_t), 1, file);
    dht->section_length = ntohs(dht->section_length);
    printf("%u\n", dht->section_length);
    fread(&(dht->huffman_information)[0],sizeof(uint8_t),1,file);
    dht->huffman_information[1]=dht->huffman_information[0]%16;
    dht->huffman_information[0]=dht->huffman_information[0]/16;
    printf("%u\n",dht->huffman_information[0]);
    printf("%u\n",dht->huffman_information[1]);
    if(dht->huffman_information[0]>1){
        printf("ERROR HUFFMAN INFORMATION\n");
        exit(EXIT_FAILURE);
    }
    /*INITIALIZE SYMBOLS NUMBER*/
    for (int i=0;i<16;i+=1){
        fread(&(dht->symbols_number)[i],sizeof(uint8_t),1,file);
        printf("%u ",dht->symbols_number[i]);
    }
    printf("\n");
    /*INITIALIZE SYMBOLS*/
    uint8_t length_symbols_table=dht->section_length-19;
    /*printf("LENGTH: %u",length_symbols_table);*/
    dht->symbols=calloc(length_symbols_table,sizeof(uint8_t));
    for (int i=0;i<length_symbols_table ;i++) {
        fread(&(dht->symbols)[i],sizeof(uint8_t),1,file);
        printf("%u ",(dht->symbols)[i]);
    }
    printf("\n");
    return dht;
}
struct SOS* EXTRACT_SOS(FILE* file){
    printf("SOS info: \n");
    /*INITIALIZE SOS STRUCTURE*/
    struct SOS* sos= malloc(sizeof(struct SOS));
    /*INITIALIZE SECTION LENGTH*/
    fread(&(sos->section_length), sizeof(uint16_t), 1, file);
    sos->section_length = ntohs(sos->section_length);
    printf("%u\n", sos->section_length);
    fread(&(sos->components_number), sizeof(uint8_t), 1, file);
    printf("N:%u\n", sos->components_number);
    sos->i_c=malloc(sos->components_number*sizeof(uint8_t));
    sos->i_h_DC=malloc(sos->components_number*sizeof(uint8_t));
    sos->i_h_AC=malloc(sos->components_number*sizeof(uint8_t));
    for (int i=0;i<sos ->components_number;i++){
        fread(&(sos->i_c)[i],sizeof(uint8_t),1,file);
        fread(&(sos->i_h_DC)[i],sizeof(uint8_t),1,file);
        printf("TT: %u\n",(sos->i_h_DC)[i]);
        sos->i_h_AC[i]=(sos->i_h_DC[i])%16;
        sos->i_h_DC[i]=(sos->i_h_DC[i])/16;
        printf("i_c: %u\n",(sos->i_c)[i]);
        printf("i_h_DC: %u\n",(sos->i_h_DC)[i]);
        printf("i_h_AC: %u\n",(sos->i_h_AC)[i]);
    }
    return sos;
}

void extract_header(struct HEADER* header,FILE* file){
    uint8_t octets;
    header->dqts=calloc(1,sizeof(struct DQTs));
    header->dqts->dqt_table=calloc(4,sizeof(struct DQT* ));
    header->dqts->dqt_counter=0;
    while(fread(&octets,sizeof(uint8_t),1,file)==1){ /*READ 1 BY 1 BYTES*/
        if (octets==0xff){ /* IF WE HAVE A BEGINNING OF A MARKER FF*/
            fread(&octets,sizeof(uint8_t),1,file); /* ADVANCE BY 1 BYTES*/
            switch (octets) {
                case 0xd8: /* IF MARKER IS SOI D8, CONTINUE */
                    printf("PASSAGE d8:\n");
                    continue;
                case 0xe0:
                    printf("PASSAGE e0:\n");
                    header->app0=EXTRACT_APP0(file);
                    break;
                case 0xdb:
                    printf("PASSAGE: db\n");
                    header->dqts->dqt_table[header->dqts->dqt_counter++]=EXTRACT_DQT(file);
                    break;
                case 0xc0:
                    printf("PASSAGE: c0\n");
                    header->sof=EXTRACT_SOF(file);
                    break;
                case 0xc4:
                    printf("PASSAGE:c4 \n");
                    header->dht=EXTRACT_DHT(file);
                    break;
                case 0xda:
                    printf("PASSAGE:da \n");
                    header->sos=EXTRACT_SOS(file);
                    break;
            }
            
        }
    }
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
    FILE *jpeg_image= fopen(argv[1],"r");
    struct HEADER* header=calloc(1,sizeof(struct HEADER));
    extract_header(header,jpeg_image);
    printf("section length:  %u ",(header->dqts->dqt_table[1])->i_q);
    printf("DQT COUNTER: %i",header->dqts->dqt_counter);
    /*printf("q_i:%u",(header->sof->quantification_table_i_q)[0]);*/
    fclose(jpeg_image);
    free(header->app0->image_type);
    free(header->dqts->dqt_table);
    free(header->sof->i_c);
    free(header->sof->quantification_table_i_q);
    free(header->sof->sampling_horizontal);
    free(header->sof->sampling_vertical);
    free(header->dht->symbols);
    return EXIT_SUCCESS;
}   