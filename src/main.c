
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/jpeg_reader.h"
#include "../include/bitstream.h"
#include "../include/bitstream_to_mcu.h"
#include "../include/extraction_mcu.h"


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
    uint8_t* pre_order_list=components_order(header->sof,header->sos);
    printf("order: %u\n",pre_order_list[0]);
    uint8_t* pre_occurrence_list=block_number_list(header->sof);
    huffnode** hufftrees=malloc(6*sizeof(huffnode*));
    hufftrees[0]=header->dhts->dht_table[0]->tree;
    hufftrees[1]=header->dhts->dht_table[1]->tree;
    hufftrees[2]=header->dhts->dht_table[2]->tree;
    hufftrees[3]=header->dhts->dht_table[3]->tree;
    hufftrees[4]=header->dhts->dht_table[2]->tree;
    hufftrees[5]=header->dhts->dht_table[3]->tree;

    
    uint8_t components_number=header->sof->components_number;
    printf("COMP: %u\n",components_number);
    uint16_t height=header->sof->height;
    uint16_t width=header->sof->width;
    uint8_t shapee[2]={0,0};
    
    // test lmcu
    LMCU* mcu=bit_stream_to_LMCU(bitstream, pre_order_list, pre_occurrence_list, hufftrees,components_number,height, width, shapee); 
    for (uint8_t i = 0; i < 64; i++) {
        printf("/%x",*(mcu->MCUs[1500]->LY[1]->content[i]));
    

        
    }    
    free_header(header);
    return EXIT_SUCCESS;
    //free MCU_lis;
}