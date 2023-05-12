
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "jpeg_reader.c"
#include <math.h>


/* - pour DC: CALCUL MAGNITUDE -> On lit octet coeff DC : 7c : en décimal 124 -> magnitude=7 : code 1111100 et code de huffman : 0 donc codage 01111100
    pour DC suivant: on encode la difference par rapport à la valeur du DC précédent ( pour la meme composante ) */


uint8_t magnitude(int16_t signed_integer){
    /*THIS FUNCTION RETURNS THE MAGNITUDE OF A SIGNED INTEGER*/

    uint16_t abs_value=abs(signed_integer);
    
    uint8_t mag_class=0;
    while (abs_value>0){
        abs_value= abs_value>>1; /*RIGHT SHIFTING TO DIVIDE BY 2 */
        mag_class++;
    }
    
    if (mag_class>11) mag_class==11;

    return mag_class;
}

char *int_to_binary(int16_t n,uint8_t magnitude) {
  
  /*CONVERTS A SIGNED INTEGER TO BINARY AND ONLY TAKES THE "MAGNITUDE" LOW-WEIGHTED BITS*/

  char *binary = (char *) malloc(sizeof(char) * (magnitude+1));
  int i = 0;
  unsigned int mask = 1 <<(magnitude-1); 
  
  while (mask > 0) {
    if ((n & mask)== mask) {
        binary[i]='1'; }
    else binary[i]='0';
    i++;
    mask >>= 1;
  }

  binary[i] = '\0';

  return binary;
}
int16_t* signed_integers_table(uint8_t magnitude){
    
    /*THIS TABLE CONTAINS ALL SIGNED INTEGERS WITHIN THE MAGNITUDE INDEX*/
    int16_t* signed_integers=malloc(pow(2,magnitude)*sizeof(int16_t));
    
    /*INITIALIZE NEGATIVE RANGES*/
    int16_t lower_neg=-(pow(2,magnitude)-1);
    int16_t higher_neg=lower_neg+(pow(2,magnitude-1)-1);
    
    /*INTIALIZE POSITIVE RANGES*/
    int16_t higher_pos=pow(2,magnitude)-1;
    int16_t lower_pos=higher_pos-pow(2,magnitude-1)+1;
    
    /*FILL THE SIGNED INTEGERS TABLE*/
    int j=0;
    for (int16_t i=lower_neg;i<=higher_neg;i++){
        signed_integers[j]=i;
        j++;
        }
    for (int16_t i=lower_pos;i<=higher_pos;i++){
        signed_integers[j]=i;
        j++;
    }
    return signed_integers;
}


int find_index_signed_integer(int16_t* signed_integers_table,int16_t signed_integer){
    /*FINDS THE INDEX OF A SIGNED INTEGER IN THE SIGNED INTEGER TABLE */
    
    int index=0;
    while (signed_integers_table[index]!=signed_integer){
        index++;
    }
    return index;
}


char** int_to_binary_table(uint8_t magnitude){
    
    /* THIS FUNCTION RETURN A TABLE CONTAINING ALL CODES FOR SIGNED INTEGERS WITHIN THE MAGNITUDE INDEX*/
    
    /*INITIALIZE THE RETURN TABLE*/
    char** binary_table=calloc(pow(2,magnitude),sizeof(char *));
    
    /*THIS TABLE CONTAINS ALL SIGNED INTEGERS WITHIN THE MAGNITUDE INDEX*/
    int16_t* signed_integers=signed_integers_table(magnitude);
    
    
    /*FILL THE BINARY TABLE CODES FOR EACH SIGNED INTEGER */
    for (int i=0;i<pow(2,magnitude);i++){
        binary_table[i]=int_to_binary(signed_integers[i],magnitude);
    }
    return binary_table;
}

char* find_code(char** binary_table,int16_t signed_integer,uint8_t magnitude){
    
    /*FINDS THE CODE RELATED TO THE SIGNED INTEGER IN THE BINARY TABLE*/
    
    char* code;
    int16_t* signed_table=signed_integers_table(magnitude);
    int index=find_index_signed_integer(signed_table,signed_integer);
    code=binary_table[index];
    return code;

}


unsigned char* bit_extraction(unsigned char buffer){
    
    /*BIT EXTRACTION FROM A BYTE*/
    unsigned char* bits =calloc(8,sizeof(int));
    for (int i=7;i>=0;i--){
        bits [7-i]=(buffer >> i) & 1;
    }
    return bits ;
} 


int* bits_codes_verification_and_position(unsigned char* bits,unsigned char** codes,uint8_t length){
    
    /*VERIFIES IF A HUFFMAN SYMBOL CORRESPONDS WITH THE BITSTREAM */
    printf("length: %u\n ",length);
    int i=0;
    int j=0;
    while (strcmp(&(codes[j][i]),"")!=0){
            printf("codes[%i][%i]: %c\n",j,i,codes[j][i]);
            printf("bits[%i]: %u\n",i,bits[i]);
            if (bits[i]!=codes[j][i]){
                j++;
                printf("j : %i\n",j);
                printf("malkach\n");
                if (j>=length) break;
            }
                else {
                i++;
                printf("lka\n");
                }
    }
    int* i_j=calloc(2,sizeof(int));
    i_j[0]=i;
    i_j[1]=j;

    return i_j;
    
}

uint8_t* get_huffman_DC_index(struct SOS* sos){
    return sos->i_h_AC;
    }

uint8_t* get_huffman_AC_index(struct SOS* sos){
    return sos->i_h_AC;
}

/*WORK TO BE DONE HERE: EXTRACT THE SYMBOLS SEQUENCE IN A GIVEN BLOCK */

unsigned char EXTRACT_DC_SYMBOL(FILE* file,uint8_t* list_DC_lengths,struct HEADER* header){
    uint8_t byte;
    fread(&byte,sizeof(uint8_t),1,file);
    printf("byte: %02x\n",byte);
    unsigned char* bits=bit_extraction(byte);
    /*for (int i=0;i<8;i++){
        printf("bit[i]= %u\n",bits[i]);
    }ok*/

    int* position_symbolindex=bits_codes_verification_and_position(bits,header->dhts->dht_table[0]->paths,header->dhts->dht_table[0]->symbols_number_total);
    printf("POSITION SYMBOLE: %i\n",position_symbolindex[1]-1);
    printf("symbole index: %i\n",position_symbolindex[1]);
    unsigned char symbol_DC=header->dhts->dht_table[0]->symbols[position_symbolindex[1]-1];
    printf("SYMBOLE: %02x\n",symbol_DC);
    return symbol_DC;
    

}


/*unsigned char* EXTRACT_AC_SYMBOLS(FILE* file,uint8_t* list_AC_lengths,struct DHT* dht){
    
    huffnode* root=create_huffnode(NULL,"");
    char** codes=huffmancodes(list_AC_lengths,root,16);
    uint8_t byte;

    }

}*/

/*void block_bitstream_to_symbols_sequence(unsigned char type_AC_DC,FILE* file,struct DHT* dht){
    unsigned char* symbols_sequence=calloc(64,sizeof(unsigned char));
    if (type_AC_DC==0){
        /*IF TYPE DHT IS DC 
        symbols_sequence[0]=EXTRACT_DC_SYMBOL(file,dht->symbols_number,dht);
        }
    else if (type_AC_DC==1){
        /*IF TYPE 
    }

        

    }*/

int main(int argc,char** argv){
    FILE *jpeg_image= fopen(argv[1],"r");
    struct HEADER* header=calloc(1,sizeof(struct HEADER));
    extract_header(header,jpeg_image);
    /*printf("paths: %s\n",header->dhts->dht_table[0]->paths[0]);*/
    /*unsigned char* bit=calloc(8,sizeof(unsigned char));*/
    /*block_bitstream_to_symbols_sequence(0,jpeg_image,header->dhts->dht_table[0]);*/
    /*uint8_t mag=magnitude(4);
    printf("magnitude=: %u\n",mag);
    char* binary_2=int_to_binary(2,2);
    char** binary_2_table=int_to_binary_table(mag);
    for (uint8_t i=0;i<pow(2,mag);i++){
        printf("TAB: %s\n",binary_2_table[i]);
    }
    char* code=find_code(binary_2_table,-3,2);
    printf("code for 2: %s\n",code);*/
    for (int i=0;i<header->dhts->dht_table[0]->symbols_number_total;i++){
        printf("paths[i]: %s \n",header->dhts->dht_table[0]->paths[i]);
    }
    unsigned char symbol_DC= EXTRACT_DC_SYMBOL(jpeg_image,header->dhts->dht_table[0]->symbols_number,header);
    printf("symbole DC: %02x\n",symbol_DC);
    fclose(jpeg_image);
    free_header(header);
    
    return EXIT_SUCCESS;
}
