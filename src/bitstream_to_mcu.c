#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct block{
    uint8_t block_type; // 0 for Y // 1 for Cb // 2 for Cr
    uint8_t* content;
} block;

typedef struct MCU{
    block* LY;
    block* LCb;
    block* LCr;
    //uint8_t S
} MCU;

typedef struct LMCU{
    MCU* MCUs;
    uint8_t* order_list;
    uint8_t* occurence_list;
    uint8_t* MCU_shape;
} LMCU;

// Function to create a block
block* createBlock(uint8_t T, uint8_t* block_content) {
    block* newBlock = malloc(sizeof(block));
    newBlock->block_type = T;
    newBlock->content = calloc(64,sizeof(uint8_t));
    for (int i = 0; i < 64; i++) {
        newBlock->content[i]=block_content[i];
    }
    return newBlock;
}

// Function to create an MCU
MCU* createMCU(block* LY, block* LCb, block* LCr/*, int S*/) {
    MCU* newMCU = malloc(sizeof(MCU));
    newMCU->LY = LY;
    newMCU->LCb = LCb;
    newMCU->LCr = LCr;
    /*newMCU->S = S;*/
    return newMCU;
}

// Function to create an LMCU
LMCU* createLMCU(MCU* MCUs, uint8_t* order_list, uint8_t* occurrence_list, uint8_t* MCU_shape) {
    LMCU* new_LMCU = malloc(sizeof(LMCU));
    new_LMCU->MCUs = MCUs;
    new_LMCU->order_list = order_list;
    new_LMCU->occurence_list = occurrence_list;
    new_LMCU->MCU_shape = MCU_shape;
    return new_LMCU;
}


int main(void){
    //test block type
    uint8_t block_content[64];
    for (int i = 0; i < 64; i++) {
        block_content[i]=i;
    }
    block* newBlock=createBlock(0,block_content);
    for (int i = 0; i < 64; i++) {
        printf("newBlock->content[%u] is %x and is %x %x\n",i,(newBlock->content[i]),(newBlock->content[i])>>4,(newBlock->content[i] & 0x0F));
    }


}