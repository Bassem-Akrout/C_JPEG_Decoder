#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>


uint8_t S(int x,int y,int16_t tab_freq[8][8]){
    /* TAB FREQ IS THE 8*8 BLOCK AFTER IZZ AND IQ */
    float sum=0;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            if (i==0 && j==0){
                sum+=(1/2)*cos((2*x+1)*i*M_PI/16)*cos((2*y+1)*j*M_PI/16)*tab_freq[i][j];
            }
            else if ((i==0 && j!=0) || (i!=0 && j==0)){
                sum+=(1/sqrt(2))*cos((2*x+1)*i*M_PI/16)*cos((2*y+1)*j*M_PI/16)*tab_freq[i][j];
            }
            else{
                sum+=cos((2*x+1)*i*M_PI/16)*cos((2*y+1)*j*M_PI/16)*tab_freq[i][j];
            }


        }
    }
    float sum_total=sum/4+128;
    if (sum_total<0) {printf("s= %f , %u\n",(sum/4)+128,0);
        return 0;}
    else if (sum_total>255) { printf("s= %f , %u\n",(sum/4)+128,255);
        return (uint8_t) 255;}
    else {printf("s= %f , %u\n",(sum/4)+128,(uint8_t)sum/4+128);
    return (uint8_t) sum_total;}

}

void idct(uint8_t out[8][8],int16_t tab_freq[8][8]){
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            uint8_t value=S(i,j,tab_freq);
            out[i][j]=value;
        }
    }
}

int main(void){
    int16_t freq[8][8]={{6,-9,-1,-1,0,0,0,0},{4,1,-1,1,1,0,0,1},{-7,0,0,-1,0,0,0,0},{2,1,-1,0,0,0,0,-1},{-1,0,0,0,0,0,0,0},{1,0,-1,0,0,0,0,0},{0,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0}};
    uint8_t out[8][8];
    printf("entier: %i\n",freq[0][1]);
    idct(out,freq);
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            printf("out[%i,%i]= %02x\n",i,j,out[i][j]);
        }
    }
    
}