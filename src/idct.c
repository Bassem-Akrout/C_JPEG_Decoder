#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define pi 3.14159265358979323846

float S(int* x,int* y,float phi[8][8]){
    float sum=0;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            if (i==0 && j==0){
                sum+=(1/2)*cos((2*(*x)+1)*i*pi/16)*cos((2*(*y)+1)*j*pi/16)*phi[i][j];
            }
            else if ((i==0 && j!=0) || (i!=0 && j==0)){
                sum+=(1/sqrt(2))*cos((2*(*x)+1)*i*pi/16)*cos((2*(*y)+1)*j*pi/16)*phi[i][j];
            }
            else{
                sum+=cos((2*(*x)+1)*i*pi/16)*cos((2*(*y)+1)*j*pi/16)*phi[i][j];
            }


        }
    }
    return sum/sqrt(16);

}

void idct(float S[8][8],uint8_t converted[8][8]){
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            uint8_t value=(uint8_t)(S[i][j]+128.0f);
            if (value<0) value=0;
            else if (value>255) value=255;
            converted[i][j]=(uint8_t) value;
        }
    }
}