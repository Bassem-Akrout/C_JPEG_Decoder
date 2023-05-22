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


void modi_mcu(iM_MCU* old_im_mcu ,iM_MCU* up_samp_im_mcu,uint8_t* S_fact ){
/*missing cases: h,v >4*/
    up_samp_im_mcu->LY=old_im_mcu->LY;
    if (S_fact[2]==1 && S_fact[3]==1){
        if(S_fact[0]==1 && S_fact[1]==1){
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
        else if((S_fact[0]==2 && S_fact[1]==2)){
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
            
            //temporary
            iM_block* left_Cb_block =malloc(sizeof(iM_block));
            iM_block* right_Cb_block=malloc(sizeof(iM_block)); 
            iM_block* left_Cr_block =malloc(sizeof(iM_block));
            iM_block* right_Cr_block=malloc(sizeof(iM_block)); 




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
        else{
            printf("CASE NOT TREATED type 0:");
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
        else{
            printf("CASE NOT TREATED type 0:");
            printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);        
        }
    
    }
    else if(((S_fact[2]==2 && S_fact[3]==2))){
        if (S_fact[0]==2 && S_fact[1]==2){
            up_samp_im_mcu->LCb=old_im_mcu->LCb;//not sure this works
            up_samp_im_mcu->LCr=old_im_mcu->LCr;           
        }
        else{
            printf("CASE NOT TREATED type 2: ");
            printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);
        }
    }

    else{
        
    }



}

iM_LMCU* up_sample(iM_LMCU* im_lmcu,struct SOF* sof){
    
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



