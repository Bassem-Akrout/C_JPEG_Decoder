
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/jpeg_reader.h"
#include "../include/bitstream.h"
#include "../include/bitstream_to_mcu.h"
#include "../include/extraction_mcu.h"
#include "../include/izz.h"
#include "../include/idct.h"
#include "../include/treatment.h"


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
    printf("test1");
    char* bitstream=bitstream_extraction();
    uint8_t* pre_order_list=components_order(header->sof,header->sos);
    
    uint8_t* pre_occurrence_list=block_number_list(header->sof);
    

    huffnode** hufftrees=malloc(2*header->sof->components_number*sizeof(huffnode*));
    if (header->sof->components_number==1){
        hufftrees[0]=header->dhts->dht_table[0]->tree;
        hufftrees[1]=header->dhts->dht_table[1]->tree;
    } else {
        for (int i=0;i<2*header->sof->components_number-2;i++){
        hufftrees[i]=header->dhts->dht_table[i]->tree;
        }
        hufftrees[2*header->sof->components_number-2]=header->dhts->dht_table[2*header->sof->components_number-4]->tree;
        hufftrees[2*header->sof->components_number-1]=header->dhts->dht_table[2*header->sof->components_number-3]->tree;
    }
    
    uint8_t components_number=header->sof->components_number;
    //printf("COMP: %u\n",components_number);
    uint16_t height=header->sof->height;
    uint16_t width=header->sof->width;
    
    // test lmcu
    LMCU* lmcu=bit_stream_to_LMCU(bitstream, pre_order_list, pre_occurrence_list, hufftrees,components_number,height, width); 
    printf("\n");
    for (uint8_t i = 0; i < 64; i++) {
        printf("/%x",*(lmcu->MCUs[0]->LY[0]->content[i]));
    }
    printf("\n");
    uint32_t total_mcu=header->sof->height/(header->sof->sampling_vertical[0]*8)*header->sof->width/(header->sof->sampling_horizontal[0]*8);
    uint8_t nb_blocks_Y=header->sof->sampling_horizontal[0]*header->sof->sampling_vertical[0];
    /*for (uint32_t i=0;i<total_mcu;i++){
        for (int j=0;j<nb_blocks_Y;j++){
            int16_t*** matrix_block=malloc(8*sizeof(int16_t**));
            for (int l=0;l<8;l++) {
                matrix_block[l]=malloc(8*sizeof(int16_t*));
                    for (int z=0;z<8;z++)
                        matrix_block[l][z]=malloc(8*sizeof(int16_t));
            }

            quantification(lmcu->MCUs[i]->LY[j]->content,header->dqts->dqt_table[0]->quantification_values);
                for (int k=0;k<64;k++) 
                printf("quant: %02x\n",*(lmcu->MCUs[i]->LY[j]->content[k]));
            printf("QUANTIFICATION OK\n");
            zz_inverse(matrix_block,lmcu->MCUs[i]->LY[j]->content);
            printf("inverse\n");
            for (int k=0;k<8;k++)
                for (int l=0;l<8;l++)
                    printf("izz: %02x\n",*(matrix_block)[k][l]);
            printf("ZIGZAG OK\n");
            uint8_t*** matrix_block_idct=malloc(8*sizeof(uint8_t**));
            for (int l=0;l<8;l++) {
                matrix_block_idct[l]=malloc(8*sizeof(uint8_t*));
                    for (int z=0;z<8;z++)
                        matrix_block_idct[l][z]=malloc(8*sizeof(uint8_t));
            }
            idct(matrix_block_idct,matrix_block);
            for (int k=0;k<8;k++)
                for (int l=0;l<8;l++)
                    printf("idct: %x\n",*(matrix_block_idct)[k][l]);
        
        }
    }*/
    printf("test00 \n");
    printf("\n");

    M_LMCU* m_lmcu= create_M_LMCU( lmcu, header->dqts , header->sof->components_number,header->sof);
       /* for (uint8_t i = 0; i < 8; i++) {
                for (uint8_t j = 0; j < 8; j++) {
        printf(" %x",*(m_lmcu->M_MCUs[1]->LCb[0]->content[i][j]));

    }*/
    
    printf("\n");
        for (uint8_t i = 0; i < 8; i++) {
                for (uint8_t j = 0; j < 8; j++) {
        printf(" %x ",*(m_lmcu->M_MCUs[1500]->LY[0]->content[i][j]));

    }
    printf("\n");
    }

    iM_LMCU* im_lmcu=create_iM_LMCU(m_lmcu,header->sof);
     for (uint8_t i = 0; i < 8; i++) {
                for (uint8_t j = 0; j < 8; j++) {
        printf(" %x ",*(im_lmcu->iM_MCUs[1500]->LY[0]->content[i][j]));

    }
    printf("\n");
    }

    free_header(header);
    return EXIT_SUCCESS;
    //free MCU_lis;
}
