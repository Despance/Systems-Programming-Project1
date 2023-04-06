#include <stdio.h>
#include <stdlib.h>

void readFile(char fileName[], char order, char type[], int size);
void hexToBin (char hex[][3], int bin[], int size);

int main(int argc, char* argv[]) {
    //readFile(argv[1], argv[2][0], argv[3], argv[4][0]-48);
    readFile("input.txt", 'l', "int", 4);
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