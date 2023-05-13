
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "jpeg_reader.c"
#include <math.h>


/* - pour DC: CALCUL MAGNITUDE -> On lit octet coeff DC : 7c : en décimal 124 -> magnitude=7 : code 1111100 et code de huffman : 0 donc codage 01111100
    pour DC suivant: on encode la difference par rapport à la valeur du DC précédent ( pour la meme composante ) */

uint8_t magnitude_char(char* code){
    return strlen(code);
}


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

int find_index_code(char** binary_table,char* code,uint8_t magnitude){
    
    /*FINDS THE INDEX OF THE CODE IN THE BINARY TABLE*/
    int index=0;
    while(strcmp(binary_table[index],code)!=0){
        index++;
    }
    return index;

}

int16_t find_signed_integer_from_code(char** binary_table,int16_t* signed_integers_table,char* code,uint8_t magnitude){
    /*FINDS THE SIGNED INTEGER IN THE SIGNED INTEGER TABLE FROM THE CORRESPODING*/

    int index=find_index_code(binary_table,code,magnitude);
    int16_t integer=signed_integers_table[index];
    printf("integer: %i\n",integer);
    if (integer<0){
        return integer+1;
    }
    return integer;
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


/*FUNCTIONS TO BE DONE : -read through bit stream while nb_elements in the block <64 and add to block  */


void add_to_block1_from_bitstream(FILE* file,int16_t* block/*CONTAINS 64 UINT16s* and *huffman_tree*/){
    /*while index<64
        read bitstream 1 by 1 and go in huffman tree : 
            when finds symbol : extracts it and : if bits_forts=0 ; does nothing else stocks nb(pdfaible)*[0] in block  
                                              magnitude= nb(pdfort)(exmple : 07 : magnitude = 7) 
                                              stock the (magnitude) next bits after converting them using the fct "find_signed_from_code"
                                               in the block   */
        
}                                              
void add_to_block_i_from_bitstream(FILE* file,int16_t* block,int16_t previous_DC){
    /*SAME THING BUT FOR DC WE EXTRACT SIGNED INT FROM MAGNITUDE CODE AND ADD IT TO THE PREVIOUS SIGNED DC INTEGER*/
}

/*WHAT NEEDS TO BE DONE NEXT : WORK ON EVERY COMPONENT (EACH BLOCK IN COMPONENT) AND DO THE SAME THING : ARGUMENTS : INDEX / DHT TABLES FROM HEADER
                                THEN WORK ON THE WHOLE BITSTREAM AND EXTRACT MCUS(EVERY MCU HAS COMPONENTS)*/

int main(int argc,char** argv){

    FILE *jpeg_image= fopen(argv[1],"r");
    struct HEADER* header=calloc(1,sizeof(struct HEADER));
    extract_header(header,jpeg_image);
    /*printf("paths: %s\n",header->dhts->dht_table[0]->paths[0]);*/
    /*unsigned char* bit=calloc(8,sizeof(unsigned char));*/
    /*block_bitstream_to_symbols_sequence(0,jpeg_image,header->dhts->dht_table[0]);*/
    uint8_t mag=magnitude(-333);
    printf("mag: %u\n",mag);
    /*printf("magnitude=: %u\n",mag);*/
    /*char* binary_2=int_to_binary(2,2);*/
    char** binary_2_table=int_to_binary_table(2);
    for (uint8_t i=0;i<pow(2,2);i++){
                printf("TAB: %s\n",binary_2_table[i]);
    }

    /*char* code=find_code(binary_2_table,-3,2);
    printf("code for 2: %s\n",code);*/
    /*for (int i=0;i<header->dhts->dht_table[0]->symbols_number_total;i++){
        printf("paths[i]: %s \n",header->dhts->dht_table[0]->paths[i]);
    }*/
    /*uint8_t magnitude=magnitude_char("1111100");
    printf("magnitude=: %u\n",magnitude);*/
    int16_t* signed_table=signed_integers_table(2);
    /*int16_t integer=find_signed_integer_from_code(binary_2_table,signed_table,"010110010",9);*/
    /*printf("integer: %i\n",integer);*/
    /*uint16_t mask=0xffff;
    uint16_t u=integer & mask;
    printf("INTEGER: %04x\n",u);*/
    /*unsigned char symb_DC=EXTRACT_DC_SYMBOL(jpeg_image,header->dhts->dht_table[0]->symbols_number,header);
    printf("symb_DC %02x\n",symb_DC);*/

    fclose(jpeg_image);
    free_header(header);

    
    return EXIT_SUCCESS;
}
