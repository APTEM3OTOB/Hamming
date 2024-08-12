#include "hamming.h"
#include <stdlib.h>
#include <string.h>
#define m 127
#define m1 120

char** hamming(char array[][m1], int length_a) {
    char** area;
    int i;

    area = (char**)malloc(length_a * sizeof(char*));
    for (i = 0; i < length_a; i++) {
        area[i] = (char*)malloc(m * sizeof(char));
        strcpy(area[i], array[i]);
    }
    return area;
}