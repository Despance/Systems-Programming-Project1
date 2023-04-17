#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))

void readFile(char fileName[], char byteOrder, char type[], int dataSize);
void hexToBin(char hex[][3], char bin[], int dataSize);
void binaryToIEEE(char* binaryNumber, int dataTypeSize);
void bitToIntSigned(char bits[], int dataSize);
void bitToIntUnsigned(char bits[], int dataSize);

FILE* output;

int main(int argc, char* argv[]) { 
    output = fopen("output.txt", "w");  
 
    if (argc == 5)  
        readFile(argv[1], argv[2][0], argv[3], (argv[4][0]-'0'));
    else 
        printf("Inputs are missing");
}

/* This function reads the input file according to byte ordering and dataSize of the data */
void readFile(char fileName[], char byteOrder, char dataType[], int dataSize) {
    FILE* input = fopen(fileName, "r");
    char hex[dataSize][3]; 
    char temp[3];

    int count = 1;
    while(fscanf(input, "%s", temp) == 1) {
        if ((count - 1) % (12 / dataSize) == 0 && count != 1)
            fprintf(output, "\n");
        
        // if the byte ordering type is BigEndian read data from beginning
        if (byteOrder == 'b') {
            strcpy(hex[0], temp);
            for (int i = 1; i < dataSize; i++) 
                fscanf(input, "%s", hex[i]);
        }
        // if the byte ordering type is LittleEndian read data from the end
        else if (byteOrder == 'l') {
            strcpy(hex[dataSize - 1], temp);
            for (int i = dataSize - 2; i >= 0; i--) 
                fscanf(input, "%s", hex[i]);
        }

        char bin[8*dataSize];
        hexToBin(hex, bin, dataSize);

        // call function acording to given dataType
        if (strcmp(dataType, "float") == 0)
            binaryToIEEE(bin, dataSize);
        else if (strcmp(dataType, "int") == 0)
            bitToIntSigned(bin, dataSize);
        else 
            bitToIntUnsigned(bin, dataSize);

        if (count % (12 / dataSize) != 0) 
            fprintf(output, " ");

        count++;
    }  
}

/* This function converts hexadecimal data to binary */
void hexToBin(char hex[][3], char bin[], int dataSize) {
    // convert hexadecimal data to binary
    for (int i = 0; i < dataSize; i++) {
        for (int j = 0; j < 2; j++) {
            int dec;
            if (hex[i][j] <= '9')
                dec = hex[i][j] - '0'; // for 0-9
            else 
                dec = hex[i][j] - 'a' + 10; // for a-f
            for (int k = 0; k < 4; k++) {
                bin[3+(i*8)+(j*4)-k] = (dec % 2) + '0'; // int to char
                dec /= 2;
            }
        }
    }
}

void binaryToIEEE(char* binaryNumber, int dataTypeSize) {
    //Calculate the boundaries and bias
    int expDigitSize = 2 + dataTypeSize*2;
    int fractionLastIndex = min(dataTypeSize*8,expDigitSize+14);
    int exponentValue = 0;
    double fractionValue = 1 ;
    long bias = pow(2,expDigitSize-1)-1;
    int sign = binaryNumber[0]-'0';

    //Calculating the exponent
    for (int i = expDigitSize+1; i > 0 ; i--)
        exponentValue += (binaryNumber[i]-'0')*pow(2,expDigitSize-i);
    
    //If the exponent is 0 the number will be Denormalized and fraction will start with 0.xxx
    if (exponentValue == 0)
        fractionValue = 0;

    //Rounding the mantissa
    int roundedDigit = expDigitSize+14;
    int roundToEven = 1;
    if (dataTypeSize>2 && binaryNumber[roundedDigit--]== '1') {
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
        if (roundToEven && binaryNumber[roundedDigit] == '1') {
            while(binaryNumber[roundedDigit]!='0')
                binaryNumber[roundedDigit--]='0';

            binaryNumber[roundedDigit]='1';
        }
    }

    //Calculating the mantissa
    for (int i = expDigitSize+1; i < fractionLastIndex; i++) {
        fractionValue+= (binaryNumber[i]-'0')*pow(2,expDigitSize-i);
        
    }
    
    //Classifying and calculating the result.
    if (exponentValue == pow(2,expDigitSize)-1) { //Special values 
        if (fractionValue == 0 || fractionValue == 1) {
            if (sign==0)
                fprintf(output, "∞");
            else
                fprintf(output, "-∞");
        }
        else
            fprintf(output, "NaN");
    }
    else if (exponentValue == 0) { // Denormalized values 
        double result = pow(-1, sign)*fractionValue/pow(2,bias-1);
        if (result==0) 
            fprintf(output, "%.0lf", result);
        else
            fprintf(output, "%.5e", result);
    }
    else {   //Normalized values
        double result = pow(-1, sign)*fractionValue*pow(2,exponentValue-bias);
        fprintf(output, "%.5lf", result);
    }
}

void bitToIntSigned(char bits[], int dataSize){
    int number = 0, power = 1;
    if (bits[0]=='0'){
        for(int i = 8*dataSize-1; i>=1 ; i--){
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

    fprintf(output, "%d", number);
}

void bitToIntUnsigned(char bits[], int dataSize){
    unsigned int number = 0, power = 1;
    for(int i = 8*dataSize-1; i>=0 ; i--){
        if(bits[i]=='1'){
            number += power;
        }
        power *=2;
    }
    fprintf(output, "%u", number);
}