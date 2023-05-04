#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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

    




    

    

    
    
 
    
    



    



    return EXIT_SUCCESS;
}