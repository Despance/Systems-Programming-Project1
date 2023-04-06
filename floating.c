#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))


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

//char input[1][33] = {"10000000000110000000000000000000"};


double BinaryToIEEE(char* binaryNumber, int dataTypeSize){

    //Calculate the boundaries and bias
    int expDigitSize = 2 + dataTypeSize*2;
    int fractionLastIndex = min(dataTypeSize*8,expDigitSize+14);
    int exponentValue = 0;
    float fractionValue = 1 ;
    int bias = pow(2,expDigitSize-1)-1;

    //Calculating the exponent
    for (int i = expDigitSize+1; i > 0 ; i--)
        exponentValue += (binaryNumber[i]-'0')*pow(2,expDigitSize-i);
    
    //If the exponent is 0 the number will be Denormalized and fraction will start with 0.xxx
    if (exponentValue == 0)
        fractionValue = 0;

    //Rounding the mantissa
    int roundedDigit = expDigitSize+14;
    int roundToEven = 1;
    if (dataTypeSize>2 && binaryNumber[roundedDigit--]== '1')
    {
        //Round Up
        for (int i = roundedDigit+2; i < dataTypeSize*8; i++)
        {
            if (binaryNumber[i] == '1')
            {
                roundToEven = 0;
                while(binaryNumber[roundedDigit]!='0')
                    binaryNumber[roundedDigit--]='0';
                
                binaryNumber[roundedDigit]='1';
                break;
            }
        }

        //Round to Even
        if (roundToEven && binaryNumber[roundedDigit] == '1')
        {
            while(binaryNumber[roundedDigit]!='0')
                binaryNumber[roundedDigit--]='0';

            binaryNumber[roundedDigit]='1';
        }
    }

    //Calculating the mantissa
    for (int i = expDigitSize+1; i < fractionLastIndex; i++)
    {
        fractionValue+= (binaryNumber[i]-'0')*pow(2,expDigitSize-i);
        
    }
    
    //Classifying and calculating the result.
    if (exponentValue == pow(2,expDigitSize)-1)//Special values
    {
        if (fractionValue == 0 || fractionValue == 1)
            return (binaryNumber[0]=='0')? 1.0/0.0:-1.0/0.0;
        else
            return 0.0/0.0;
    }
    else if (exponentValue == 0) // Denormalized values
    {
        //!! Denormalized values must be printied using %e identifier.
        double result = fractionValue/pow(2,bias-1);
        return (binaryNumber[0]=='0')? result:-result;
    }
    else    //Normalized values
    {
        float result = fractionValue*pow(2,exponentValue-bias);
        return (binaryNumber[0]=='0')? result:-result;
    }

}



int main(){
    for (int i = 0; i < 9; i++)
    {
        double result = BinaryToIEEE(input[i],4);
        printf("%f \n",result);
    }
    
    
}