
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h> /* for the function ntohs() */
#include "bitstream.c"
#include "bitstream_to_mcu.c"
#include "extraction_mcu.c"


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
   
    create_stream(jpeg_image);
    char* bitstream=bitstream_extraction();
    uint8_t* order=components_order(header->sof,header->sos);
    uint8_t* occurence=block_number_list(header->sof);
    huffnode** hufftrees=malloc(4*sizeof(huffnode*));
    for (int i=0;i<header->dhts->dht_counter;i++){
        hufftrees[i]=header->dhts->dht_table[i]->tree;
        hufftrees[i+2]=header->dhts->dht_table[i+2]->tree;
    }
    uint8_t components_number=header->sof->components_number;
    uint16_t height=header->sof->height;
    uint16_t width=header->sof->width;
    uint8_t* shape=NULL;
    LMCU* list_mcu=bit_stream_to_LMCU(bitstream,order,occurence,hufftrees,components_number,height,width,shape);
    printf("Cb\n");
    for (uint8_t i = 0; i < 64; i++) {
        printf("/%x",*(list_mcu->MCUs[0]->LCb[0]->content[i]));

        
    }    
    printf("Cr\n");
    for (uint8_t i = 0; i < 64; i++) {
        printf("/%x",*(list_mcu->MCUs[0]->LCr[0]->content[i]));      
    }    
    printf("\nDONE\n");





    
    
    /*for (int i = 0; i < header->dhts->dht_table[1]->symbols_number_total; i++) {
    printf("path :%s \n", header->dhts->dht_table[1]->paths[i]);}*/
    fclose(jpeg_image);
    free_header(header);
    return EXIT_SUCCESS;
}