#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/idct.h"
#include "../include/jpeg_reader.h"


/* THIS FUNCTION UPSAMPLES AN MCU: BY DUPLICATION ! */
void horizontal_duplication(iM_block* initial,iM_block* left_block, iM_block* right_block){

    left_block->content = calloc(8,sizeof(uint8_t**));
    right_block->content = calloc(8,sizeof(uint8_t**));
    for (int i = 0; i < 8; i++) {
        left_block->content[i]=calloc(8,sizeof(uint8_t*));
        right_block->content[i]=calloc(8,sizeof(uint8_t*));
        for (int j = 0; j < 8; j++) {
            left_block->content[i][j]=calloc(8,sizeof(uint8_t));
            right_block->content[i][j]=calloc(8,sizeof(uint8_t));
        }
    }
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 4; j++){
            left_block->content[i][2*j]=   initial->content[i][j];
            left_block->content[i][2*j+1]= initial->content[i][j];
            
            right_block->content[i][2*j]=  initial->content[i][4+j];
            right_block->content[i][2*j+1]=initial->content[i][4+j];
        }
    }


}

void vertical_duplication(iM_block* initial,iM_block* upper_block, iM_block* lower_block){

    upper_block->content = calloc(8,sizeof(uint8_t**));
    lower_block->content = calloc(8,sizeof(uint8_t**));
    for (int i = 0; i < 8; i++) {
        upper_block->content[i]=calloc(8,sizeof(uint8_t*));
        lower_block->content[i]=calloc(8,sizeof(uint8_t*));
        for (int j = 0; j < 8; j++) {
            upper_block->content[i][j]=calloc(8,sizeof(uint8_t));
            lower_block->content[i][j]=calloc(8,sizeof(uint8_t));
        }
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
            upper_block->content[2*i][j]=   initial->content[i][j];
            upper_block->content[2*i+1][j]= initial->content[i][j];
            
            lower_block->content[2*i][j]=  initial->content[4+i][j];
            lower_block->content[2*i+1][j]=initial->content[4+i][j];
        }
    }
}

void one_to_three_horizontal_duplication(iM_block* initial,iM_block* left_block,iM_block* mid_block ,iM_block* right_block){

    left_block->content = calloc(8,sizeof(uint8_t**));
    mid_block->content = calloc(8,sizeof(uint8_t**));
    right_block->content = calloc(8,sizeof(uint8_t**));
    for (int i = 0; i < 8; i++) {
        left_block->content[i]=calloc(8,sizeof(uint8_t*));
        mid_block->content[i]=calloc(8,sizeof(uint8_t*));
        right_block->content[i]=calloc(8,sizeof(uint8_t*));
        for (int j = 0; j < 8; j++) {
            left_block->content[i][j]=calloc(8,sizeof(uint8_t));
            mid_block->content[i][j]=calloc(8,sizeof(uint8_t));
            right_block->content[i][j]=calloc(8,sizeof(uint8_t));
        }
    }
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 2; j++){
            left_block->content[i][3*j]=   initial->content[i][j];
            left_block->content[i][3*j+1]= initial->content[i][j];
            left_block->content[i][3*j+2]= initial->content[i][j];
            
            mid_block->content[i][3*j+1]=   initial->content[i][j+3];
            mid_block->content[i][3*j+2]= initial->content[i][j+3];
            mid_block->content[i][3*j+3]= initial->content[i][j+3];

            
            right_block->content[i][3*j+2]=  initial->content[i][j+6];
            right_block->content[i][3*j+3]=initial->content[i][j+6];
            right_block->content[i][3*j+4]=initial->content[i][j+6];
        }
        left_block->content[i][6]= initial->content[i][2];
        left_block->content[i][7]= initial->content[i][2];
        mid_block->content[i][0]=  initial->content[i][2];
        mid_block->content[i][7]=  initial->content[i][4];
        right_block->content[i][0]=initial->content[i][4];
        right_block->content[i][1]=initial->content[i][4];
    }


}

void one_to_three_vertical_duplication(iM_block* initial,iM_block* upper_block,iM_block* mid_block ,iM_block* lower_block){

    upper_block->content = calloc(8,sizeof(uint8_t**));
    mid_block->content = calloc(8,sizeof(uint8_t**));
    lower_block->content = calloc(8,sizeof(uint8_t**));
    for (int i = 0; i < 8; i++) {
        upper_block->content[i]=calloc(8,sizeof(uint8_t*));
        mid_block->content[i]=calloc(8,sizeof(uint8_t*));
        lower_block->content[i]=calloc(8,sizeof(uint8_t*));
        for (int j = 0; j < 8; j++) {
            upper_block->content[i][j]=calloc(8,sizeof(uint8_t));
            mid_block->content[i][j]=calloc(8,sizeof(uint8_t));
            lower_block->content[i][j]=calloc(8,sizeof(uint8_t));
        }
    }
    for (int j = 0; j < 8; j++){
        for (int i = 0; i <2; i++){
            upper_block->content[3*i][j]=   initial->content[i][j];
            upper_block->content[3*i+1][j]= initial->content[i][j];
            upper_block->content[3*i+2][j]= initial->content[i][j];
            
            mid_block->content[3*i+1][j]=   initial->content[i+3][j];
            mid_block->content[3*i+2][j]= initial->content[i+3][j];
            mid_block->content[3*i+3][j]= initial->content[i+3][j];
            
            lower_block->content[3*i+2][j]=  initial->content[i+6][j];
            lower_block->content[3*i+3][j]=initial->content[i+6][j];
            lower_block->content[3*i+4][j]=initial->content[i+6][j];
        }
        upper_block->content[6][j]= initial->content[2][j];
        upper_block->content[7][j]= initial->content[2][j];
        mid_block->content[0][j]=  initial ->content[2][j];
        mid_block->content[7][j]=  initial ->content[4][j];
        lower_block->content[0][j]=initial ->content[4][j];
        lower_block->content[1][j]=initial ->content[4][j];
    }


}

void one_to_four_vertical_duplication(iM_block* initial,iM_block* upper_block,iM_block* upper_mid_block,iM_block* lower_mid_block, iM_block* lower_block){

    upper_block->content = calloc(8,sizeof(uint8_t**));
    upper_mid_block->content = calloc(8,sizeof(uint8_t**));
    lower_mid_block->content = calloc(8,sizeof(uint8_t**));
    lower_block->content = calloc(8,sizeof(uint8_t**));
    for (int i = 0; i < 8; i++) {
        upper_block->content[i]=calloc(8,sizeof(uint8_t*));
        upper_mid_block->content[i] = calloc(8,sizeof(uint8_t**));
        lower_mid_block->content[i] = calloc(8,sizeof(uint8_t**));
        lower_block->content[i]=calloc(8,sizeof(uint8_t*));
        for (int j = 0; j < 8; j++) {
            upper_block->content[i][j]=calloc(8,sizeof(uint8_t));
            upper_mid_block->content[i][j] = calloc(8,sizeof(uint8_t**));
            lower_mid_block->content[i][j] = calloc(8,sizeof(uint8_t**));
            lower_block->content[i][j]=calloc(8,sizeof(uint8_t));
        }
    }
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 8; j++){

            upper_block->content[4*i][j]=       initial->content[i][j];
            upper_block->content[4*i+1][j]=     initial->content[i][j];
            upper_block->content[4*i+2][j]=     initial->content[i][j];
            upper_block->content[4*i+3][j]=     initial->content[i][j];

            upper_mid_block->content[4*i][j]=   initial->content[2+i][j];
            upper_mid_block->content[4*i+1][j]= initial->content[2+i][j];
            upper_mid_block->content[4*i+2][j]= initial->content[2+i][j];
            upper_mid_block->content[4*i+3][j]= initial->content[2+i][j];

            lower_mid_block->content[4*i][j]=   initial->content[4+i][j];
            lower_mid_block->content[4*i+1][j]= initial->content[4+i][j];
            lower_mid_block->content[4*i+2][j]= initial->content[4+i][j];
            lower_mid_block->content[4*i+3][j]= initial->content[4+i][j];

            lower_block->content[4*i][j]=       initial->content[6+i][j];
            lower_block->content[4*i+1][j]=     initial->content[6+i][j];
            lower_block->content[4*i+2][j]=     initial->content[6+i][j];
            lower_block->content[4*i+3][j]=     initial->content[6+i][j];


        }
    }
}

void one_to_four_horizontal_duplication(iM_block* initial,iM_block* left_block,iM_block* mid_left_block,iM_block* mid_right_block, iM_block* right_block){

    left_block->content = calloc(8,sizeof(uint8_t**));
    mid_left_block->content = calloc(8,sizeof(uint8_t**));
    mid_right_block->content = calloc(8,sizeof(uint8_t**));
    right_block->content = calloc(8,sizeof(uint8_t**));
    for (int i = 0; i < 8; i++) {
        left_block->content[i]=calloc(8,sizeof(uint8_t*));
        mid_left_block->content[i] = calloc(8,sizeof(uint8_t**));
        mid_right_block->content[i] = calloc(8,sizeof(uint8_t**));
        right_block->content[i]=calloc(8,sizeof(uint8_t*));
        for (int j = 0; j < 8; j++) {
            left_block->content[i][j]=calloc(8,sizeof(uint8_t));
            mid_left_block->content[i][j] = calloc(8,sizeof(uint8_t**));
            mid_right_block->content[i][j] = calloc(8,sizeof(uint8_t**));
            right_block->content[i][j]=calloc(8,sizeof(uint8_t));
        }
    }
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 2; j++){


            left_block->content[i][4*j]=        initial->content[i][j];
            left_block->content[i][4*j+1]=      initial->content[i][j];
            left_block->content[i][4*j+2]=      initial->content[i][j];
            left_block->content[i][4*j+3]=      initial->content[i][j];

            mid_left_block->content[i][4*j]=    initial->content[i][2+j];
            mid_left_block->content[i][4*j+1]=  initial->content[i][2+j];
            mid_left_block->content[i][4*j+2]=  initial->content[i][2+j];
            mid_left_block->content[i][4*j+3]=  initial->content[i][2+j];
            
            mid_right_block->content[i][4*j]=   initial->content[i][4+j];
            mid_right_block->content[i][4*j+1]= initial->content[i][4+j];    
            mid_right_block->content[i][4*j+2]= initial->content[i][4+j];
            mid_right_block->content[i][4*j+3]= initial->content[i][4+j];
            
            right_block->content[i][4*j]=       initial->content[i][6+j];
            right_block->content[i][4*j+1]=     initial->content[i][6+j];    
            right_block->content[i][4*j+2]=     initial->content[i][6+j];
            right_block->content[i][4*j+3]=     initial->content[i][6+j];

        }
    }
}

void modi_LCb(iM_MCU* old_im_mcu ,iM_MCU* up_samp_im_mcu,uint8_t h, uint8_t v ){

}
void modi_mcu(iM_MCU* old_im_mcu ,iM_MCU* up_samp_im_mcu,uint8_t* S_fact ){
//does the up_sampling step taking into consideration stocked in S_fact like this {h1,v1,h2,v2,h3,v3}
    up_samp_im_mcu->LY=old_im_mcu->LY;
    if (S_fact[2]==1 && S_fact[3]==1){
        if      (S_fact[0]==1 && S_fact[1]==1){
            up_samp_im_mcu->LCb=old_im_mcu->LCb;
            up_samp_im_mcu->LCr=old_im_mcu->LCr;

        }
        else if ((S_fact[0]==2 && S_fact[1]==1)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));

            iM_block* left_Cb_block =malloc(sizeof(iM_block));
            iM_block* right_Cb_block=malloc(sizeof(iM_block));
            iM_block* left_Cr_block =malloc(sizeof(iM_block));
            iM_block* right_Cr_block=malloc(sizeof(iM_block));
            horizontal_duplication(old_im_mcu->LCb[0],left_Cb_block,right_Cb_block);
            horizontal_duplication(old_im_mcu->LCr[0],left_Cr_block,right_Cr_block);

            up_samp_im_mcu->LCb[0]=left_Cb_block;
            up_samp_im_mcu->LCb[1]=right_Cb_block;
            up_samp_im_mcu->LCr[0]=left_Cr_block;
            up_samp_im_mcu->LCr[1]=right_Cr_block;
            //free all old

       
        }
        else if ((S_fact[0]==1 && S_fact[1]==2)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));

            iM_block* upper_Cb_block =malloc(sizeof(iM_block));
            iM_block* lower_Cb_block=malloc(sizeof(iM_block));
            iM_block* upper_Cr_block =malloc(sizeof(iM_block));
            iM_block* lower_Cr_block=malloc(sizeof(iM_block));
            vertical_duplication(old_im_mcu->LCb[0],upper_Cb_block,lower_Cb_block);
            vertical_duplication(old_im_mcu->LCr[0],upper_Cr_block,lower_Cr_block);


            up_samp_im_mcu->LCb[0]=upper_Cb_block;
            up_samp_im_mcu->LCb[1]=lower_Cb_block;
            up_samp_im_mcu->LCr[0]=upper_Cr_block;
            up_samp_im_mcu->LCr[1]=lower_Cr_block;
        //free all old

       
        }
        else if ((S_fact[0]==1 && S_fact[1]==4)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));

            iM_block* upper_Cb_Block=malloc(sizeof(iM_block));
            iM_block* upper_mid_Cb_Block=malloc(sizeof(iM_block));
            iM_block* lower_mid_Cb_Block=malloc(sizeof(iM_block));
            iM_block* lower_Cb_Block=malloc(sizeof(iM_block));

            iM_block* upper_Cr_Block=malloc(sizeof(iM_block));
            iM_block* upper_mid_Cr_Block=malloc(sizeof(iM_block));
            iM_block* lower_mid_Cr_Block=malloc(sizeof(iM_block));
            iM_block* lower_Cr_Block=malloc(sizeof(iM_block));

            one_to_four_vertical_duplication(old_im_mcu->LCb[0],upper_Cb_Block, upper_mid_Cb_Block, lower_mid_Cb_Block,  lower_Cb_Block);
            one_to_four_vertical_duplication(old_im_mcu->LCr[0],upper_Cr_Block, upper_mid_Cr_Block, lower_mid_Cr_Block,  lower_Cr_Block);

            up_samp_im_mcu->LCb[0]=upper_Cb_Block;
            up_samp_im_mcu->LCb[1]=upper_mid_Cb_Block;
            up_samp_im_mcu->LCb[2]=lower_mid_Cb_Block;
            up_samp_im_mcu->LCb[3]=lower_Cb_Block;

            up_samp_im_mcu->LCr[0]=upper_Cr_Block;
            up_samp_im_mcu->LCr[1]=upper_mid_Cr_Block;
            up_samp_im_mcu->LCr[2]=lower_mid_Cr_Block;
            up_samp_im_mcu->LCr[3]=lower_Cr_Block;

        }
        else if ((S_fact[0]==4 && S_fact[1]==1)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));

            iM_block* left_Cb_Block=malloc(sizeof(iM_block));
            iM_block* mid_left_Cb_Block=malloc(sizeof(iM_block));
            iM_block* mid_right_Cb_Block=malloc(sizeof(iM_block));
            iM_block* right_Cb_Block=malloc(sizeof(iM_block));

            iM_block* left_Cr_Block=malloc(sizeof(iM_block));
            iM_block* mid_left_Cr_Block=malloc(sizeof(iM_block));
            iM_block* mid_right_Cr_Block=malloc(sizeof(iM_block));
            iM_block* right_Cr_Block=malloc(sizeof(iM_block));

            one_to_four_horizontal_duplication( old_im_mcu->LCb[0], left_Cb_Block,mid_left_Cb_Block, mid_right_Cb_Block,  right_Cb_Block);
            one_to_four_horizontal_duplication( old_im_mcu->LCr[0], left_Cr_Block,mid_left_Cr_Block, mid_right_Cr_Block,  right_Cr_Block);

            up_samp_im_mcu->LCb[0]=left_Cb_Block;
            up_samp_im_mcu->LCb[1]=mid_left_Cb_Block;
            up_samp_im_mcu->LCb[2]=mid_right_Cb_Block;
            up_samp_im_mcu->LCb[3]=right_Cb_Block;

            up_samp_im_mcu->LCr[0]=left_Cr_Block;
            up_samp_im_mcu->LCr[1]=mid_left_Cr_Block;
            up_samp_im_mcu->LCr[2]=mid_right_Cr_Block;
            up_samp_im_mcu->LCr[3]=right_Cr_Block;

        }
        else if ((S_fact[0]==2 && S_fact[1]==2)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));

            iM_block* upper_left_Cb_block =malloc(sizeof(iM_block));
            iM_block* upper_right_Cb_block=malloc(sizeof(iM_block));
            iM_block* lower_left_Cb_block =malloc(sizeof(iM_block));
            iM_block* lower_right_Cb_block=malloc(sizeof(iM_block));  

            iM_block* upper_left_Cr_block =malloc(sizeof(iM_block));
            iM_block* upper_right_Cr_block=malloc(sizeof(iM_block));
            iM_block* lower_left_Cr_block =malloc(sizeof(iM_block));
            iM_block* lower_right_Cr_block=malloc(sizeof(iM_block));
            
            //temporary
            iM_block* left_Cb_block =malloc(sizeof(iM_block));
            iM_block* right_Cb_block=malloc(sizeof(iM_block)); 
            iM_block* left_Cr_block =malloc(sizeof(iM_block));
            iM_block* right_Cr_block=malloc(sizeof(iM_block)); 



            //one hori then two verti
            horizontal_duplication(old_im_mcu->LCb[0],left_Cb_block,right_Cb_block);
            horizontal_duplication(old_im_mcu->LCr[0],left_Cr_block,right_Cr_block);

            vertical_duplication(left_Cb_block, upper_left_Cb_block ,lower_left_Cb_block);
            vertical_duplication(right_Cb_block,upper_right_Cb_block,lower_right_Cb_block);

            vertical_duplication(left_Cr_block, upper_left_Cr_block ,lower_left_Cr_block);
            vertical_duplication(right_Cr_block,upper_right_Cr_block,lower_right_Cr_block);
            
            up_samp_im_mcu->LCb[0]=upper_left_Cb_block;
            up_samp_im_mcu->LCb[1]=upper_right_Cb_block;
            up_samp_im_mcu->LCb[2]=lower_left_Cb_block;
            up_samp_im_mcu->LCb[3]=lower_right_Cb_block;   

            up_samp_im_mcu->LCr[0]=upper_left_Cr_block;
            up_samp_im_mcu->LCr[1]=upper_right_Cr_block;
            up_samp_im_mcu->LCr[2]=lower_left_Cr_block;
            up_samp_im_mcu->LCr[3]=lower_right_Cr_block;

            free(left_Cb_block);
            free(right_Cb_block); 
            free(left_Cr_block);
            free(right_Cr_block); 

            free(old_im_mcu->LCb);
            free(old_im_mcu->LCr); //il faut free l'interieur aussi
        }
        else if ((S_fact[0]==2 && S_fact[1]==4)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            //blocs are ordered this way
            /*
            0 1
            2 3
            4 5
            6 7
            */
            iM_block* Cb_0=malloc(sizeof(iM_block)); 
            iM_block* Cb_1=malloc(sizeof(iM_block));
            iM_block* Cb_2=malloc(sizeof(iM_block));
            iM_block* Cb_3=malloc(sizeof(iM_block));
            iM_block* Cb_4=malloc(sizeof(iM_block));
            iM_block* Cb_5=malloc(sizeof(iM_block));
            iM_block* Cb_6=malloc(sizeof(iM_block));
            iM_block* Cb_7=malloc(sizeof(iM_block));

            iM_block* Cr_0=malloc(sizeof(iM_block)); 
            iM_block* Cr_1=malloc(sizeof(iM_block));
            iM_block* Cr_2=malloc(sizeof(iM_block));
            iM_block* Cr_3=malloc(sizeof(iM_block));
            iM_block* Cr_4=malloc(sizeof(iM_block));
            iM_block* Cr_5=malloc(sizeof(iM_block));
            iM_block* Cr_6=malloc(sizeof(iM_block));
            iM_block* Cr_7=malloc(sizeof(iM_block));




            //temporary
            iM_block* left_Cb_block =malloc(sizeof(iM_block));
            iM_block* right_Cb_block=malloc(sizeof(iM_block)); 

            iM_block* left_Cr_block =malloc(sizeof(iM_block));
            iM_block* right_Cr_block=malloc(sizeof(iM_block)); 

            horizontal_duplication(old_im_mcu->LCb[0],left_Cb_block,right_Cb_block);
            horizontal_duplication(old_im_mcu->LCr[0],left_Cr_block,right_Cr_block);

            one_to_four_vertical_duplication(left_Cb_block , Cb_0 ,Cb_2,Cb_4,Cb_6);
            one_to_four_vertical_duplication(left_Cr_block , Cr_0 ,Cr_2,Cr_4,Cr_6);
            one_to_four_vertical_duplication(right_Cb_block, Cb_1 ,Cb_3,Cb_5,Cb_7);
            one_to_four_vertical_duplication(right_Cr_block, Cr_1 ,Cr_3,Cr_5,Cr_7);

            up_samp_im_mcu->LCb[0]=Cb_0;
            up_samp_im_mcu->LCb[1]=Cb_1;
            up_samp_im_mcu->LCb[2]=Cb_2;
            up_samp_im_mcu->LCb[3]=Cb_3;
            up_samp_im_mcu->LCb[4]=Cb_4;
            up_samp_im_mcu->LCb[5]=Cb_5;
            up_samp_im_mcu->LCb[6]=Cb_6;
            up_samp_im_mcu->LCb[7]=Cb_7;

            up_samp_im_mcu->LCr[0]=Cr_0;
            up_samp_im_mcu->LCr[1]=Cr_1;
            up_samp_im_mcu->LCr[2]=Cr_2;
            up_samp_im_mcu->LCr[3]=Cr_3;
            up_samp_im_mcu->LCr[4]=Cr_4;
            up_samp_im_mcu->LCr[5]=Cr_5;
            up_samp_im_mcu->LCr[6]=Cr_6;
            up_samp_im_mcu->LCr[7]=Cr_7;

            free(left_Cb_block);
            free(right_Cb_block); 
            free(left_Cr_block);
            free(right_Cr_block); 

            free(old_im_mcu->LCb);
            free(old_im_mcu->LCr); //il faut free l'interieur aussi
    

        }
        else if ((S_fact[0]==4 && S_fact[1]==2)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            //blocs are ordered this way
            /*
            0 1 2 3
            4 5 6 7             
            */

            iM_block* Cb_0=malloc(sizeof(iM_block)); 
            iM_block* Cb_1=malloc(sizeof(iM_block));
            iM_block* Cb_2=malloc(sizeof(iM_block));
            iM_block* Cb_3=malloc(sizeof(iM_block));
            iM_block* Cb_4=malloc(sizeof(iM_block));
            iM_block* Cb_5=malloc(sizeof(iM_block));
            iM_block* Cb_6=malloc(sizeof(iM_block));
            iM_block* Cb_7=malloc(sizeof(iM_block));

            iM_block* Cr_0=malloc(sizeof(iM_block)); 
            iM_block* Cr_1=malloc(sizeof(iM_block));
            iM_block* Cr_2=malloc(sizeof(iM_block));
            iM_block* Cr_3=malloc(sizeof(iM_block));
            iM_block* Cr_4=malloc(sizeof(iM_block));
            iM_block* Cr_5=malloc(sizeof(iM_block));
            iM_block* Cr_6=malloc(sizeof(iM_block));
            iM_block* Cr_7=malloc(sizeof(iM_block));




            //temporary
            iM_block* upper_Cb_block =malloc(sizeof(iM_block));
            iM_block* lower_Cb_block=malloc(sizeof(iM_block)); 

            iM_block* upper_Cr_block =malloc(sizeof(iM_block));
            iM_block* lower_Cr_block=malloc(sizeof(iM_block)); 

            vertical_duplication(old_im_mcu->LCb[0],upper_Cb_block,lower_Cb_block);
            vertical_duplication(old_im_mcu->LCr[0],upper_Cr_block,lower_Cr_block);

            one_to_four_horizontal_duplication(upper_Cb_block, Cb_0 ,Cb_1,Cb_2,Cb_3);
            one_to_four_horizontal_duplication(upper_Cr_block, Cr_0 ,Cr_1,Cr_2,Cr_3);
            one_to_four_horizontal_duplication(lower_Cb_block, Cb_4 ,Cb_5,Cb_6,Cb_7);
            one_to_four_horizontal_duplication(lower_Cr_block, Cr_4 ,Cr_5,Cr_6,Cr_7);

            up_samp_im_mcu->LCb[0]=Cb_0;
            up_samp_im_mcu->LCb[1]=Cb_1;
            up_samp_im_mcu->LCb[2]=Cb_2;
            up_samp_im_mcu->LCb[3]=Cb_3;
            up_samp_im_mcu->LCb[4]=Cb_4;
            up_samp_im_mcu->LCb[5]=Cb_5;
            up_samp_im_mcu->LCb[6]=Cb_6;
            up_samp_im_mcu->LCb[7]=Cb_7;

            up_samp_im_mcu->LCr[0]=Cr_0;
            up_samp_im_mcu->LCr[1]=Cr_1;
            up_samp_im_mcu->LCr[2]=Cr_2;
            up_samp_im_mcu->LCr[3]=Cr_3;
            up_samp_im_mcu->LCr[4]=Cr_4;
            up_samp_im_mcu->LCr[5]=Cr_5;
            up_samp_im_mcu->LCr[6]=Cr_6;
            up_samp_im_mcu->LCr[7]=Cr_7;

            free(upper_Cb_block);
            free(upper_Cr_block); 
            free(lower_Cb_block);
            free(lower_Cr_block); 

            free(old_im_mcu->LCb);
            free(old_im_mcu->LCr); //il faut free l'interieur aussi
    
        }
        else if ((S_fact[0]==3 && S_fact[1]==1)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));

            iM_block* left_Cb_block =malloc(sizeof(iM_block));
            iM_block* mid_Cb_block =malloc(sizeof(iM_block));
            iM_block* right_Cb_block=malloc(sizeof(iM_block));
            iM_block* left_Cr_block =malloc(sizeof(iM_block));
            iM_block* mid_Cr_block =malloc(sizeof(iM_block));
            iM_block* right_Cr_block=malloc(sizeof(iM_block));
            one_to_three_horizontal_duplication(old_im_mcu->LCb[0],left_Cb_block,mid_Cb_block,right_Cb_block);
            one_to_three_horizontal_duplication(old_im_mcu->LCr[0],left_Cr_block,mid_Cr_block,right_Cr_block);

            up_samp_im_mcu->LCb[0]=left_Cb_block;
            up_samp_im_mcu->LCb[1]=mid_Cb_block;
            up_samp_im_mcu->LCb[2]=right_Cb_block;
            up_samp_im_mcu->LCr[0]=left_Cr_block;
            up_samp_im_mcu->LCr[1]=mid_Cr_block;
            up_samp_im_mcu->LCr[2]=right_Cr_block;
            //free all old

        }
        else if ((S_fact[0]==1 && S_fact[1]==3)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));

            iM_block* upper_Cb_block =malloc(sizeof(iM_block));
            iM_block* mid_Cb_block =malloc(sizeof(iM_block));
            iM_block* lower_Cb_block=malloc(sizeof(iM_block));
            iM_block* upper_Cr_block =malloc(sizeof(iM_block));
            iM_block* mid_Cr_block =malloc(sizeof(iM_block));
            iM_block* lower_Cr_block=malloc(sizeof(iM_block));
            one_to_three_vertical_duplication(old_im_mcu->LCb[0],upper_Cb_block,mid_Cb_block,lower_Cb_block);
            one_to_three_vertical_duplication(old_im_mcu->LCr[0],upper_Cr_block,mid_Cr_block,lower_Cr_block);

            up_samp_im_mcu->LCb[0]=upper_Cb_block;
            up_samp_im_mcu->LCb[1]=mid_Cb_block;
            up_samp_im_mcu->LCb[2]=lower_Cb_block;
            up_samp_im_mcu->LCr[0]=upper_Cr_block;
            up_samp_im_mcu->LCr[1]=mid_Cr_block;
            up_samp_im_mcu->LCr[2]=lower_Cr_block;
            //free all old

        }
        else{
            printf("CASE NOT TREATED type 0:");
            printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);        
        }
    }

    else if(S_fact[2]==2 && S_fact[3]==1){
        if (S_fact[0]==2 && S_fact[1]==1){
            up_samp_im_mcu->LCb=old_im_mcu->LCb;
            up_samp_im_mcu->LCr=old_im_mcu->LCr;
        }
        else if(S_fact[0]==2 && S_fact[1]==2){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));

            iM_block* upper_left_Cb_block =malloc(sizeof(iM_block));
            iM_block* upper_right_Cb_block=malloc(sizeof(iM_block));
            iM_block* upper_left_Cr_block =malloc(sizeof(iM_block));
            iM_block* upper_right_Cr_block=malloc(sizeof(iM_block));

            iM_block* lower_left_Cb_block =malloc(sizeof(iM_block));
            iM_block* lower_right_Cb_block=malloc(sizeof(iM_block));
            iM_block* lower_left_Cr_block =malloc(sizeof(iM_block));
            iM_block* lower_right_Cr_block=malloc(sizeof(iM_block));

            vertical_duplication(old_im_mcu->LCb[0],upper_left_Cb_block,lower_left_Cb_block);
            vertical_duplication(old_im_mcu->LCb[1],upper_right_Cb_block,lower_right_Cb_block);

            vertical_duplication(old_im_mcu->LCr[0],upper_left_Cr_block,lower_left_Cr_block);
            vertical_duplication(old_im_mcu->LCr[1],upper_left_Cr_block,lower_right_Cr_block);
            
            up_samp_im_mcu->LCb[0]=upper_left_Cb_block;
            up_samp_im_mcu->LCb[1]=upper_right_Cb_block;
            up_samp_im_mcu->LCb[2]=lower_left_Cb_block;
            up_samp_im_mcu->LCb[3]=lower_right_Cb_block;   

            up_samp_im_mcu->LCr[0]=upper_left_Cr_block;
            up_samp_im_mcu->LCr[1]=upper_right_Cr_block;
            up_samp_im_mcu->LCr[2]=lower_left_Cr_block;
            up_samp_im_mcu->LCr[3]=lower_right_Cr_block;


            free(old_im_mcu->LCb);
            free(old_im_mcu->LCr); //il faut free l'interieur aussi
        }
        else if((S_fact[0]==4 && S_fact[1]==1)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            iM_block* left_Cb_Block=malloc(sizeof(iM_block));
            iM_block* mid_left_Cb_Block=malloc(sizeof(iM_block));
            iM_block* mid_right_Cb_Block=malloc(sizeof(iM_block));
            iM_block* right_Cb_Block=malloc(sizeof(iM_block));

            iM_block* left_Cr_Block=malloc(sizeof(iM_block));
            iM_block* mid_left_Cr_Block=malloc(sizeof(iM_block));
            iM_block* mid_right_Cr_Block=malloc(sizeof(iM_block));
            iM_block* right_Cr_Block=malloc(sizeof(iM_block));

            horizontal_duplication( old_im_mcu->LCb[0], left_Cb_Block,mid_left_Cb_Block );
            horizontal_duplication( old_im_mcu->LCb[1], mid_right_Cb_Block,  right_Cb_Block);

            horizontal_duplication( old_im_mcu->LCr[0], left_Cr_Block,mid_left_Cr_Block);
            horizontal_duplication( old_im_mcu->LCr[1], mid_right_Cr_Block,  right_Cr_Block);

            up_samp_im_mcu->LCb[0]=left_Cb_Block;
            up_samp_im_mcu->LCb[1]=mid_left_Cb_Block;
            up_samp_im_mcu->LCb[2]=mid_right_Cb_Block;
            up_samp_im_mcu->LCb[3]=right_Cb_Block;

            up_samp_im_mcu->LCr[0]=left_Cr_Block;
            up_samp_im_mcu->LCr[1]=mid_left_Cr_Block;
            up_samp_im_mcu->LCr[2]=mid_right_Cr_Block;
            up_samp_im_mcu->LCr[3]=right_Cr_Block;

        }        
        else{
            printf("CASE NOT TREATED type 1:");
            printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);        
        }
    }
    
    else if(S_fact[2]==1 && S_fact[3]==2){
        if (S_fact[0]==1 && S_fact[1]==2){
            up_samp_im_mcu->LCb=old_im_mcu->LCb;
            up_samp_im_mcu->LCr=old_im_mcu->LCr;
        }
        else if(S_fact[0]==2 && S_fact[1]==2){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));

            iM_block* upper_left_Cb_block =malloc(sizeof(iM_block));
            iM_block* upper_right_Cb_block=malloc(sizeof(iM_block));
            iM_block* upper_left_Cr_block =malloc(sizeof(iM_block));
            iM_block* upper_right_Cr_block=malloc(sizeof(iM_block));

            iM_block* lower_left_Cb_block =malloc(sizeof(iM_block));
            iM_block* lower_right_Cb_block=malloc(sizeof(iM_block));
            iM_block* lower_left_Cr_block =malloc(sizeof(iM_block));
            iM_block* lower_right_Cr_block=malloc(sizeof(iM_block));

            horizontal_duplication(old_im_mcu->LCb[0],upper_left_Cb_block,upper_right_Cb_block);
            horizontal_duplication(old_im_mcu->LCb[1],lower_left_Cb_block,lower_right_Cb_block);
            
            horizontal_duplication(old_im_mcu->LCr[0],upper_left_Cr_block,upper_right_Cr_block);
            horizontal_duplication(old_im_mcu->LCr[1],lower_left_Cr_block,lower_right_Cr_block);
            
            up_samp_im_mcu->LCb[0]=upper_left_Cb_block;
            up_samp_im_mcu->LCb[1]=upper_right_Cb_block;
            up_samp_im_mcu->LCb[2]=lower_left_Cb_block;
            up_samp_im_mcu->LCb[3]=lower_right_Cb_block;   

            up_samp_im_mcu->LCr[0]=upper_left_Cr_block;
            up_samp_im_mcu->LCr[1]=upper_right_Cr_block;
            up_samp_im_mcu->LCr[2]=lower_left_Cr_block;
            up_samp_im_mcu->LCr[3]=lower_right_Cr_block;


            free(old_im_mcu->LCb);
            free(old_im_mcu->LCr); //il faut free l'interieur aussi
        }
        else if((S_fact[0]==1 && S_fact[1]==4)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            iM_block* upper_Cb_Block=malloc(sizeof(iM_block));
            iM_block* upper_mid_Cb_Block=malloc(sizeof(iM_block));
            iM_block* lower_mid_Cb_Block=malloc(sizeof(iM_block));
            iM_block* lower_Cb_Block=malloc(sizeof(iM_block));

            iM_block* upper_Cr_Block=malloc(sizeof(iM_block));
            iM_block* upper_mid_Cr_Block=malloc(sizeof(iM_block));
            iM_block* lower_mid_Cr_Block=malloc(sizeof(iM_block));
            iM_block* lower_Cr_Block=malloc(sizeof(iM_block));

            vertical_duplication(old_im_mcu->LCb[0],upper_Cb_Block,      upper_mid_Cb_Block);
            vertical_duplication(old_im_mcu->LCb[1],lower_mid_Cb_Block,  lower_Cb_Block);

            vertical_duplication(old_im_mcu->LCr[0],upper_Cr_Block,      upper_mid_Cr_Block);
            vertical_duplication(old_im_mcu->LCr[1],lower_mid_Cr_Block,  lower_Cr_Block);

            up_samp_im_mcu->LCb[0]=upper_Cb_Block;
            up_samp_im_mcu->LCb[1]=upper_mid_Cb_Block;
            up_samp_im_mcu->LCb[2]=lower_mid_Cb_Block;
            up_samp_im_mcu->LCb[3]=lower_Cb_Block;

            up_samp_im_mcu->LCr[0]=upper_Cr_Block;
            up_samp_im_mcu->LCr[1]=upper_mid_Cr_Block;
            up_samp_im_mcu->LCr[2]=lower_mid_Cr_Block;
            up_samp_im_mcu->LCr[3]=lower_Cr_Block;

        }
        else{
            printf("CASE NOT TREATED type 2:");
            printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);        
        }
    
    }
 


    else if(((S_fact[2]==3 && S_fact[3]==1))){
        if  (S_fact[0]==3 && S_fact[1]==1){
            up_samp_im_mcu->LCb=old_im_mcu->LCb;
            up_samp_im_mcu->LCr=old_im_mcu->LCr;
        }
        else{
            printf("CASE NOT TREATED type 3:");
            printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);        
        }
    }

    else if(((S_fact[2]==1 && S_fact[3]==3))){
        if  (S_fact[0]==1 && S_fact[1]==3){
            up_samp_im_mcu->LCb=old_im_mcu->LCb;
            up_samp_im_mcu->LCr=old_im_mcu->LCr;
        }
        else{
            printf("CASE NOT TREATED type 4:");
            printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);        
        }
    }
    
    else{
        printf("CASE NOT TREATED type 4:");
        printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);       
    }



}

iM_LMCU* up_sample(iM_LMCU* im_lmcu,struct SOF* sof){
//this puts the up_sampled version of im_lmcu in result and returns it (adding iM_MCUs and all the steps) 
    iM_LMCU* result;
    uint8_t* S_fact;
    if (sof->components_number==3){
        result=malloc(sizeof(iM_LMCU));
        result->iM_MCUs=calloc(im_lmcu->MCU_counter,sizeof(iM_MCU*));
        result->MCU_counter=im_lmcu->MCU_counter;
        S_fact=malloc(2*(sof->components_number)*sizeof(uint8_t));

        for (uint8_t i=0;i<3;i++){
            S_fact[2*i]=sof->sampling_horizontal[i];
            S_fact[2*i+1]=sof->sampling_vertical[i];
        }
    
    
    for (uint32_t i=0;i<im_lmcu->MCU_counter;i++){
        result->iM_MCUs[i]=malloc(sizeof(iM_MCU));
        modi_mcu(im_lmcu->iM_MCUs[i] ,result->iM_MCUs[i],S_fact);
        //free(im_lmcu->iM_MCUs[i]);//il faut  pas tjrs free LCb/LCr et pas leurs blocs mais pas LY
    }   

    }

    return result;
}



