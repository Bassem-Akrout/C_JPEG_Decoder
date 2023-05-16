#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "huffmann.c"

typedef struct block{
    uint8_t block_type; // 0 for Y // 1 for Cb // 2 for Cr
    uint8_t* content;
} block;

typedef struct MCU{
    block** LY;
    block** LCb;
    block** LCr;
    //uint8_t S
} MCU;

typedef struct LMCU{
    MCU** MCUs;
    uint8_t* order_list;
    uint8_t* occurrence_list;
    uint8_t* MCU_shape;
} LMCU;

// Function to create a block
block* createBlock(uint8_t T, int8_t* block_content) {
    block* newBlock = malloc(sizeof(block));
    newBlock->block_type = T;
    newBlock->content = calloc(64,sizeof(uint8_t));
    for (int i = 0; i < 64; i++) {
        newBlock->content[i]=block_content[i];
    }
    return newBlock;
}

// Function to create an MCU
MCU* createMCU(block** LY, block** LCb, block** LCr/*, int S*/) {
    MCU* newMCU = malloc(sizeof(MCU));
    newMCU->LY = LY;
    newMCU->LCb = LCb;
    newMCU->LCr = LCr;
    /*newMCU->S = S;*/
    return newMCU;
}

// Function to create an LMCU
LMCU* createLMCU(MCU** MCUs, uint8_t* order_list, uint8_t* occurrence_list, uint8_t* MCU_shape) {
    LMCU* new_LMCU = malloc(sizeof(LMCU));
    new_LMCU->MCUs = MCUs;
    new_LMCU->order_list = order_list;
    new_LMCU->occurrence_list = occurrence_list;
    new_LMCU->MCU_shape = MCU_shape;
    return new_LMCU;
}



int8_t find_code(char* index, uint8_t magnitude) {
    int8_t decimal = 0;
    uint8_t power = magnitude - 1;

    if (index[0] == '0') {
        decimal = -((1<<magnitude) - 1);

    }

    for (int i = 0; i < magnitude; i++) {
        char digit = index[i];
        if (digit == '1') {
            decimal += (1<<power);
        }
        power -= 1;
    }

    return decimal;
}

uint8_t find_position(uint8_t *order_list, uint8_t i  ) {
    for (uint8_t index = 0; index < 3; index++) {
        if (order_list[index] == i) {
            return index;
        }
    }
    return -1; // Element not found, returning a special value (-1)
}

uint8_t* typess(uint8_t *occurence_list, uint8_t *order_list) {
    uint8_t i0= order_list[0];
    uint8_t i1= order_list[1];
    uint8_t i2= order_list[2];
    uint8_t j0= occurence_list[i0];
    uint8_t j1= occurence_list[i1];
    uint8_t j2= occurence_list[i2];
    uint8_t* types = calloc((j0 + j1 + j2) , sizeof(uint8_t));
    for (uint8_t i = 0; i < j0; i++){
        types[i]=i0;
    }
    for (uint8_t i =i0; i < j0+j1; i++){
        types[i]=i1;
    }
    for (uint8_t i = j1+j2; i < j0+j1+j2; i++){
        types[i]=i2;
    }
    return types;
    
}


uint8_t first_occurrence(uint8_t i, uint8_t *lst, uint32_t lst_length) {
    for (uint8_t index = 0; index < lst_length; index++) {
        if (lst[index] == i) {
            return index;
        }
    }
    return -1; // Element not found, returning a special value (-1)
}




LMCU* bit_stream_to_LMCU(char* BS, uint8_t* pre_order_list, uint8_t* pre_occurrence_list, huffnode** hufftrees,uint8_t components_number,uint8_t height, uint8_t width, uint8_t* shapee) {
    uint8_t i = 0;
    uint8_t* order_list =calloc(3,sizeof(uint8_t));
    uint8_t* occurrence_list =calloc(3,sizeof(uint8_t));
    if (components_number==1){
        order_list[0] = 1;
        order_list[1] = 0;
        order_list[2] = 0;
        occurrence_list[0] = pre_occurrence_list[0];
        occurrence_list[1] = 0;
        occurrence_list[2] = 0;
    }
    else{
        order_list[0] = pre_order_list[0];
        order_list[1] = pre_order_list[1];
        order_list[2] = pre_order_list[2];
        occurrence_list[0] = pre_occurrence_list[0];
        occurrence_list[1] = pre_occurrence_list[1];
        occurrence_list[2] = pre_occurrence_list[2];
    }

    uint8_t* types = typess(occurrence_list, order_list);
    uint8_t MCU_counter = 0;
    uint32_t blocks_in_MCU = 0;
    for (int k = 0; k < 3; k++) {
        blocks_in_MCU += occurrence_list[k];
    }    
    uint32_t total_nbr_MCU = (uint32_t)((uint32_t)(width * height) / (blocks_in_MCU) * 64);
    MCU** MCU_list = calloc(total_nbr_MCU,sizeof(MCU*));

    uint8_t MCU_detector;
    block** list_of_blocks = malloc(blocks_in_MCU * sizeof(block*));
    uint8_t block_counter;
    int8_t * block_list = calloc(64 , sizeof(int8_t));
    uint8_t type_;    
    uint8_t block_detector;
    huffnode* root ;
    uint8_t null ;
    uint8_t mag ;
    int8_t DCy, DCb, DCr;
    int8_t NotDC;
    int8_t Ac ;
    block** Y_list = malloc(occurrence_list[0] * sizeof(block*));
    block** Cb_list = malloc(occurrence_list[1] * sizeof(block*));
    block** Cr_list = malloc(occurrence_list[2] * sizeof(block*));
    
    MCU* new_MCU = malloc(sizeof(MCU)) ;
    int Y_counter ,Cb_counter,Cr_counter;
    char* index;
    while (i < strlen(BS) - 7) {
        MCU_detector = 0;      
        //one mcu
        while (MCU_detector < blocks_in_MCU) {
            block_counter = 0;
            type_ = types[MCU_detector];
            block_detector = 0;
            //one block
            while (block_detector < 64) {
                if (block_detector == 0) {
                    // DC
                    root = hufftrees[2 * type_];
                    while (!(root->left == NULL && root->right == NULL)) {//sure ?
                        char bit = BS[i++];
                        if (bit == '0') {
                            root = root->left;
                        } else {
                            root = root->right;
                        }
                    }
                    null=0;
                    mag=0;
                    if (root->S != 0) {
                        null = (root->S )>>4;
                        mag = (root->S) &(0x0f);
                    }
                    index = malloc((mag + 1) * sizeof(char));//// IL FAUT free juste apres
                    for (int j = 0; j < mag; j++) {
                        index[j] = BS[i++];
                    }
                    index[mag] = '\0';

                    
                    if (MCU_counter == 0 && (MCU_detector == first_occurrence(0, types, blocks_in_MCU) ||
                        MCU_detector == first_occurrence(1, types, blocks_in_MCU) ||
                        MCU_detector == first_occurrence(2, types, blocks_in_MCU))) {
                        //DC0
                        if (type_ == 0) {
                            DCy = find_code(index, mag);
                            block_list[block_detector] = DCy;
                        } else if (type_ == 1) {
                            DCb = find_code(index, mag);
                            block_list[block_detector] = DCb;
                        } else {
                            DCr = find_code(index, mag);
                            block_list[block_detector] = DCr;
                        }
                        
                    } else {
                        //DCi
                        NotDC = find_code(index, mag);//if mag==0 index = "" and find_code(index, mag)=0
                        if (type_ == 0) {
                            DCy += NotDC;
                            block_list[block_detector] = DCy;
                        } else if (type_ == 1) {
                            DCb += NotDC;
                            block_list[block_detector] = DCb;
                        } else {
                            DCr += NotDC;
                            block_list[block_detector] = DCr;
                        }
                    }
                    free(index);
                    block_detector++;
                } 
                else {
                    // AC
                    root = hufftrees[2 * type_ + 1];
                    while (!(root->left == NULL && root->right == NULL)) {
                        char bit = BS[i++];
                        if (bit == '0') {
                            root = root->left;
                        } else {
                            root = root->right;
                        }
                    }
                    null = 0;
                    mag = 0;
                    if (root->S != 0) {
                        null = (root->S )>>4;
                        mag = (root->S) &(0x0f);
                    }
                    if (null == 0 && mag == 0){
                        //EOB
                        for (int j = block_detector; j < 64; j++) {
                            block_list[block_detector++] = 0;
                        }
                    }
                    else{
                        for (int j = 0; j < null; j++) {
                            block_list[block_detector++] = 0;
                        }
                        if (mag != 0) {
                            index = malloc((mag + 1) * sizeof(char));
                            for (int j = 0; j < mag; j++) {
                                index[j] = BS[i++];
                            }
                            index[mag] = '\0';
                            Ac = find_code(index, mag);
                            free(index);
                            block_list[block_detector++] = Ac;
                        } else {
                            block_list[block_detector++] = 0;
                        }
                    }
                }
        }
        list_of_blocks[MCU_detector] =createBlock(type_,block_list);
        MCU_detector++;
        block_counter++;
    }

 
    Y_counter = 0;
    Cb_counter = 0;
    Cr_counter = 0;

    for (uint32_t counter = 0; counter < blocks_in_MCU; counter++) {
        if (types[counter] == 0) {
            Y_list[Y_counter++] = list_of_blocks[counter];
        }
        else if (types[counter] == 1) {
            Cb_list[Cb_counter++] = list_of_blocks[counter];
        } 
        else {
            Cr_list[Cr_counter++] = list_of_blocks[counter];
        }
    }
    
    new_MCU->LY = Y_list;
    new_MCU->LCb = Cb_list;
    new_MCU->LCr = Cr_list;
    MCU_list[MCU_counter] = new_MCU;
    MCU_counter++;
}

LMCU* MCU_lis = malloc(sizeof(LMCU));
MCU_lis->order_list = order_list;
MCU_lis->occurrence_list = occurrence_list;
MCU_lis->MCU_shape = shapee;
MCU_lis->MCUs = MCU_list;

return MCU_lis;
}


int main(void){
    //test block type
    /*uint8_t block_content[64];
    for (int i = 0; i < 64; i++) {
        block_content[i]=i;
    }
    block* newBlock=createBlock(0,block_content);*/
    /*
    //test MCU_type
    block* LY[2];
    
    
    block** LCb =calloc(1,occurence_list[1],sizeof(block));
    block* LCr[1];

    uint8_t block_content[64];
    //block* newBlock;
    for (int i = 0; i < 64; i++) {
        block_content[i]=i; 
    }
    LY[0]=createBlock(0,block_content);

    for (int i = 0; i < 64; i++) {
        block_content[i]=(i+64); 
    }

    LCb[0]=createBlock(0,block_content);
    
    for (int i = 0; i < 64; i++) {
        printf("voici LCb[%i] expected %i but is %u \n",i,i+64,LCb[0]->content[i]);
    }*/

    //MCU*  new_mcu = createMCU( LY,  LCb,  LCr)
    /*//test of find code
    char* index ="11111";
    int8_t code=find_code(index,5);
    printf("code is %d \n",code);*/
    
    //test of typess
    /*uint8_t occurence_list[]={4,2,2};
    uint8_t order_list[]={2,1,0};
    uint8_t* types=typess(occurence_list, order_list);
    for (uint8_t i = 0; i < 8; i++){
        printf("%u,; ",types[i]);
    }
    printf("\n");*/
    //test of find_position
    /*uint8_t order_list[]={2,1,0}; 
    printf("find_position(order_list,0): %u \n",find_position(order_list,5));*/


}