#include "huffmann.c"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "jpeg_reader.c"




unsigned char* bit_extraction(unsigned char buffer){
    unsigned char* bit=calloc(8,sizeof(int));
    for (int i=7;i>=0;i--){
        bit[7-i]=(buffer >> i) & 1;
    }
    return bit;
} 

int* bits_codes_verification_and_position(unsigned char* bits,char** codes){
    int i=0;
    int j=0;
    while (strcmp(&(codes[j][i]),"")!=0){
            if (bits[i]!=codes[j][i]){
                j++;
            }
            else{
                i++;
            }
    }
    int* i_j=calloc(2,sizeof(int));
    i_j[0]=i;
    i_j[1]=j;

    return i_j;
    
}

unsigned char EXTRACT_DC_SYMBOL(FILE* file,uint8_t* list_DC_lengths,struct DHT* dht){
      
    huffnode* root=create_huffnode(NULL,"");
    char** codes=huffmancodes(list_DC_lengths,root,16);
    uint8_t byte;
    fread(&byte,sizeof(uint8_t),1,file);
    unsigned char* bits=bit_extraction(byte);
    int* position_symbolindex=bits_codes_verification_and_position(bits,codes);
    printf("POSITION SYMBOLE: %i\n",position_symbolindex[1]-1);
    unsigned char symbol_DC=dht->symbols[0];
    printf("SYMBOLE: %02x\n",symbol_DC);
    return symbol_DC;

}


/*unsigned char* EXTRACT_AC_SYMBOLS(FILE* file,uint8_t* list_AC_lengths,struct DHT* dht){
    
    huffnode* root=create_huffnode(NULL,"");
    char** codes=huffmancodes(list_AC_lengths,root,16);
    uint8_t byte;

    }

}*/

void block_bitstream_to_symbols_sequence(unsigned char type_AC_DC,FILE* file,struct DHT* dht){
    unsigned char* symbols_sequence=calloc(64,sizeof(unsigned char));
    if (type_AC_DC==0){
        /*IF TYPE DHT IS DC */
        symbols_sequence[0]=EXTRACT_DC_SYMBOL(file,dht->symbols_number,dht);
        }
    else if (type_AC_DC==1){
        /*IF TYPE */
    }

        

    }

int main(int* argc,char** argv){

    FILE *jpeg_image= fopen(argv[1],"r");
    struct HEADER* header=calloc(1,sizeof(struct HEADER));
    extract_header(header,jpeg_image);
    uint8_t pos;
    fread(&pos,sizeof(uint8_t),1,jpeg_image);
    unsigned char* bit=calloc(8,sizeof(unsigned char));
    for (int i=7;i>=0;i--){
        bit[7-i]=(pos >> i) & 1;
    }
    block_bitstream_to_symbols_sequence(0,jpeg_image,header->dhts->dht_table[0]);

    fclose(jpeg_image);
    free_header(header);
    return EXIT_SUCCESS;
}
