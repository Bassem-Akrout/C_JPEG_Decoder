#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/idct.h"
#include "../include/jpeg_reader.h"


/* THIS FUNCTION UPSAMPLES AN MCU: BY DUPLICATION ! */



void modi_mcu(iM_MCU* old_im_mcu ,iM_MCU* up_samp_im_mcu,uint8_t* S_fact ){



 /*   up_samp_im_mcu->LY=old_im_mcu->LY;

    if (S_fact[2]==1 && S_fact[3]==1  ){
        if(S_fact[0]==1 && S_fact[1]==1){
            up_samp_im_mcu->LCb=old_im_mcu->LCb;//not sure this works
            up_samp_im_mcu->LCr=old_im_mcu->LCr;
        }
        else if ((S_fact[0]==2 && S_fact[1]==1)||(S_fact[0]==1 && S_fact[1]==2)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCb[0]=old_im_mcu->LCb[0];
            up_samp_im_mcu->LCb[1]=old_im_mcu->LCb[0];
            up_samp_im_mcu->LCr[0]=old_im_mcu->LCr[0];
            up_samp_im_mcu->LCr[1]=old_im_mcu->LCr[0];            
        }
        else if((S_fact[0]==2 && S_fact[1]==2)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCb[0]=old_im_mcu->LCb[0];
            up_samp_im_mcu->LCb[1]=old_im_mcu->LCb[0];   
            up_samp_im_mcu->LCb[2]=old_im_mcu->LCb[0];
            up_samp_im_mcu->LCb[3]=old_im_mcu->LCb[0];   
            up_samp_im_mcu->LCr[0]=old_im_mcu->LCr[0];
            up_samp_im_mcu->LCr[1]=old_im_mcu->LCr[0];   
            up_samp_im_mcu->LCr[2]=old_im_mcu->LCr[0];
            up_samp_im_mcu->LCr[3]=old_im_mcu->LCr[0];   
            free(old_im_mcu->LCb);
            free(old_im_mcu->LCr);
        }
        else{
            printf("CASE NOT TREATED type 0:");
            printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);        
        }

    }
    else if((S_fact[2]==1 && S_fact[3]==2)){
        if (S_fact[0]==1 && S_fact[1]==2){
            up_samp_im_mcu->LCb=old_im_mcu->LCb;//not sure this works
            up_samp_im_mcu->LCr=old_im_mcu->LCr;
        }
        else if (S_fact[0]==2 && S_fact[1]==2){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCb[0]=old_im_mcu->LCb[0];
            up_samp_im_mcu->LCb[2]=old_im_mcu->LCb[0];
            up_samp_im_mcu->LCb[1]=old_im_mcu->LCb[1];
            up_samp_im_mcu->LCb[3]=old_im_mcu->LCb[1];
            up_samp_im_mcu->LCr[0]=old_im_mcu->LCr[0];
            up_samp_im_mcu->LCr[2]=old_im_mcu->LCr[0];
            up_samp_im_mcu->LCr[1]=old_im_mcu->LCr[1];
            up_samp_im_mcu->LCr[3]=old_im_mcu->LCr[1];
            free(old_im_mcu->LCb);
            free(old_im_mcu->LCr);
        }
        else{
            printf("CASE NOT TREATED type 0:");
            printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);
        }
    }
    else if((S_fact[2]==2 && S_fact[3]==1)){
        if (S_fact[0]==2 && S_fact[1]==1){
            up_samp_im_mcu->LCb=old_im_mcu->LCb;//not sure this works
            up_samp_im_mcu->LCr=old_im_mcu->LCr;
        }
        else if (S_fact[0]==2 && S_fact[1]==2){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCb[0]=old_im_mcu->LCb[0];
            up_samp_im_mcu->LCb[1]=old_im_mcu->LCb[0];
            up_samp_im_mcu->LCb[2]=old_im_mcu->LCb[1];
            up_samp_im_mcu->LCb[3]=old_im_mcu->LCb[1];
            up_samp_im_mcu->LCr[0]=old_im_mcu->LCr[0];
            up_samp_im_mcu->LCr[1]=old_im_mcu->LCr[0];
            up_samp_im_mcu->LCr[2]=old_im_mcu->LCr[1];
            up_samp_im_mcu->LCr[3]=old_im_mcu->LCr[1];
            free(old_im_mcu->LCb);
            free(old_im_mcu->LCr);        
        }
        else{
            printf("CASE NOT TREATED type 1: ");
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
        printf("CASE NOT TREATED type 3: ");
        printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);
    }*/
    up_samp_im_mcu->LY=old_im_mcu->LY;
    if (S_fact[2]==1 && S_fact[3]==1){
        if(S_fact[0]==1 && S_fact[1]==1){
            up_samp_im_mcu->LCb=old_im_mcu->LCb;//not sure this works
            up_samp_im_mcu->LCr=old_im_mcu->LCr;

        }
        else if ((S_fact[0]==2 && S_fact[1]==1)){
            printf("passaaaaaaaaaaaage \n");
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));

            iM_block* left_Cb_block =malloc(sizeof(iM_block));
            iM_block* right_Cb_block=malloc(sizeof(iM_block));
            iM_block* left_Cr_block =malloc(sizeof(iM_block));
            iM_block* right_Cr_block=malloc(sizeof(iM_block));

            left_Cb_block->content = calloc(8,sizeof(uint8_t**));
            left_Cr_block->content = calloc(8,sizeof(uint8_t**));
            right_Cb_block->content = calloc(8,sizeof(uint8_t**));
            right_Cr_block->content = calloc(8,sizeof(uint8_t**));



            for (int i = 0; i < 8; i++) {
                left_Cb_block->content[i]=calloc(8,sizeof(uint8_t*));
                left_Cr_block->content[i]=calloc(8,sizeof(uint8_t*));
                right_Cb_block->content[i]=calloc(8,sizeof(uint8_t*));
                right_Cr_block->content[i]=calloc(8,sizeof(uint8_t*));



                for (int j = 0; j < 8; j++) {
                    left_Cb_block->content[i][j]=calloc(8,sizeof(uint8_t));
                    left_Cr_block->content[i][j]=calloc(8,sizeof(uint8_t));
                    right_Cb_block->content[i][j]=calloc(8,sizeof(uint8_t));
                    right_Cr_block->content[i][j]=calloc(8,sizeof(uint8_t));
                }
            }

            for (int i = 0; i < 8; i++){
                for (int j = 0; j < 4; j++){
                    left_Cb_block->content[i][2*j]=old_im_mcu->   LCb[0]->content[i][j];
                    left_Cb_block->content[i][2*j+1]=old_im_mcu-> LCb[0]->content[i][j];
                    left_Cr_block->content[i][2*j]=old_im_mcu->   LCr[0]->content[i][j];
                    left_Cr_block->content[i][2*j+1]=old_im_mcu-> LCr[0]->content[i][j];
                    
                    right_Cb_block->content[i][2*j]=old_im_mcu->  LCb[0]->content[i][4+j];
                    right_Cb_block->content[i][2*j+1]=old_im_mcu->LCb[0]->content[i][4+j];
                    right_Cr_block->content[i][2*j]=old_im_mcu->  LCr[0]->content[i][4+j];
                    right_Cr_block->content[i][2*j+1]=old_im_mcu->LCr[0]->content[i][4+j];

                }
            }
            for(int i=0;i<8;i++){
                for(int j=0;j<8;j++){
                    printf("%02x /",*(left_Cb_block->content[i][j]));
                }
                for(int j=0;j<8;j++){
                    printf("%02x /",*(right_Cb_block->content[i][j]));
                }
            }
            printf("\n");
        up_samp_im_mcu->LCb[0]=left_Cb_block;
        up_samp_im_mcu->LCb[1]=right_Cb_block;
        up_samp_im_mcu->LCr[0]=left_Cr_block;
        up_samp_im_mcu->LCr[1]=right_Cr_block;
        //free all old

       
        }
        else if((S_fact[0]==2 && S_fact[1]==2)){
            up_samp_im_mcu->LCb= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));
            up_samp_im_mcu->LCr= calloc(S_fact[0]* S_fact[1],sizeof(iM_block*));

            free(old_im_mcu->LCb);
            free(old_im_mcu->LCr);
        }
        else{
            printf("CASE NOT TREATED type 0:");
            printf("%ux%u %ux%u %ux%u \n",S_fact[0],S_fact[1],S_fact[2],S_fact[3],S_fact[4],S_fact[5]);        
        }


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



