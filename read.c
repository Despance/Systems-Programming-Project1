#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))

void readFile(char fileName[], char order, char type[], int size);
void hexToBin (char hex[][3], int bin[], int size);
int bitToIntSigned(char bits[32]);
unsigned int bitToIntUnsigned(char bits[32]);

int main(int argc, char* argv[]) {
    //readFile(argv[1], argv[2][0], argv[3], argv[4][0]-48);
    //readFile("input.txt", 'l', "int", 4);
    char bits[32] = "10110000001101000001100001100001"; // unsigned 2956204129
    bitToIntUnsigned(bits);
    return 0;
}

/* This function reads the input file according to byte ordering and size of the data */
void readFile(char fileName[], char order, char type[], int size) {
    FILE* input = fopen(fileName, "r");
    char hex[size][3]; 

    while(!feof(input)) {
        for (int i = 0; i < size; i++) {
            // if the byte ordering type is BigEndian read data from beginning
            if (order == 'b') {
                for (int j = 0; j < size; j++) 
                    fscanf(input, "%s", hex[j]);
            }
            // if the byte ordering type is LittleEndian read data from the end
            else if (order == 'l') {
                for (int j = size - 1; j >= 0; j--) 
                fscanf(input, "%s", hex[j]);
            }

            int bin[8*size];
            hexToBin(hex, bin, size);
        }
    }
}

/* This function converts hexadecimal data to binary */
void hexToBin (char hex[][3], int bin[], int size) {
    // convert hexadecimal data to binary
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 2; j++) {
            int dec;
            if (hex[i][j] <= 57)
                dec = hex[i][j] - 48; // for 0-9
            else 
                dec = hex[i][j] - 87; // for a-f
            for (int k = 0; k < 4; k++) {
                bin[3+(i*8)+(j*4)-k] = dec % 2;
                dec /= 2;
            }
        }
    }
}


int bitToIntSigned(char bits[32]){
    int number = 0, power = 1;
    if (bits[0]=='0'){
        for(int i = 31; i>=1 ; i--){
            if(bits[i]=='1'){
                number += power;
            }
            power *=2;
        }
    }
    else if(bits[0]=='1'){
        for(int i = 31; i>=1 ; i--){
            if(bits[i]=='0'){
                number += power;
            }
            power *=2;
        }
        number+=1;
        number*=-1;
    }
    else{
        printf("Invalid first bit.");
    }
    return number;
}

unsigned int bitToIntUnsigned(char bits[32]){
    unsigned int number = 0, power = 1;
    for(int i = 31; i>=0 ; i--){
        if(bits[i]=='1'){
            number += power;
        }
        power *=2;
    }
    return number;
}





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

