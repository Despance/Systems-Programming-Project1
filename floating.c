#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))

/*
char input[9][33] ={
"01000000000000011001000011110000",
"11111111111111110000000000000011",
"01111111111000000000000000000000",
"11111111111000000000000000000000",
"00000000000000000000000000000000",
"10000000000000000000000000000000",
"10000000000110000000000000000000",
"00000000000000000000000000000000",
"00000000000000000000000000000000"};
*/
char input[1][33] = {"10000000000110000000000000000000"};


float BinaryToIEEE(char* binaryNumber, int dataTypeSize){

    int expDigitSize = 2 + dataTypeSize*2;
    int fractionLastIndex = min(dataTypeSize*8,expDigitSize+14);
    int exponentValue = 0;
    float fractionValue = 1;

    //Exponent
    for (int i = expDigitSize+1; i > 0 ; i--)
    {
        exponentValue = (binaryNumber[i]-'0')*pow(2,expDigitSize-i);
    }

    //Mantissa Rounding
    int roundedDigit = expDigitSize+14;
    if (dataTypeSize>2 && binaryNumber[roundedDigit--]== '1')
    {
        int roundToEven = 1;
        
        for (int i = roundedDigit+2; i < dataTypeSize*8; i++)
        {
            if (binaryNumber[i] == '1')
            {
                roundToEven = 0;
                
                while(binaryNumber[roundedDigit]!='0'){
                    binaryNumber[roundedDigit--]='0';
                }
                binaryNumber[roundedDigit]='1';
                break;
            }
            
        }
        if (roundToEven && binaryNumber[roundedDigit] == '1')
        {
            while(binaryNumber[roundedDigit]!='0'){
                    binaryNumber[roundedDigit--]='0';
                }
            binaryNumber[roundedDigit]='1';
        }
        
        
    }
    //Mantissa Calculating
    for (int i = expDigitSize+1; i < fractionLastIndex; i++)
    {
        fractionValue+= (binaryNumber[i]-'0')*pow(2,expDigitSize-i);
        
    }
    int bias = pow(2,expDigitSize-1)-1;

    float result = fractionValue*pow(2,exponentValue-bias);
    return (binaryNumber[0]=='0')? result:-result;
}



int main(){
    for (int i = 0; i < 1; i++)
    {
        float result = BinaryToIEEE(input[i],4);
        printf("%f - ",result);
    }
    
    
}