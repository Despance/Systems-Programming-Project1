#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readFile(char fileName[], char order, int size);
void hexToBin (char hex[][3], int bin[], int size);

int main(int argc, char* argv[]) {
    //readFile(argv[1], argv[2][0], argv[3][0]);
    readFile("input.txt", 'l', 4);
}

void readFile(char fileName[], char order, int size) {
    FILE* input = fopen(fileName, "r");
    char hex[size][3]; 

    while(!feof(input)) {
        if (order == 'b') {
            for (int i = 0; i < size; i++) 
                fscanf(input, "%s", hex[i]);
        }
        else {
            for (int i = size - 1; i >= 0; i--) 
                fscanf(input, "%s", hex[i]);
        }
    }
        int bin[8*size];
        hexToBin(hex, bin, size);
}

void hexToBin (char hex[][3], int bin[], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 2; j++) {
            int dec;
            if (hex[i][j] <= 57)
                dec = hex[i][j] - 48;
            else 
                dec = hex[i][j] - 87;
            for (int k = 0; k < 4; k++) {
                bin[3+(i*8)+(j*4)-k] = dec % 2;
                dec /= 2;
            }
        }
    }
}