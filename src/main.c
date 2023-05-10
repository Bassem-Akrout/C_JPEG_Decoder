#include "../include/jpeg_reader.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
    printf("%u\n",header->sof->section_length);
    fclose(jpeg_image);
    free_header(header);
    return EXIT_SUCCESS;
}