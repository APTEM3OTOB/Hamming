//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Made by APTEM 3OTOB %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
#include "hamming.h"
#include <stdlib.h>
#include <string.h>
#define m 64
#define m1 70

char** hamming(char bin_array[][m1], int length_a) {
    char** hammings_method = (char**)malloc(length_a * sizeof(char*));
    for (int i = 0; i < length_a; i++) {
        hammings_method[i] = (char*)malloc(m * sizeof(char));
        strcpy(hammings_method[i], bin_array[i]);
    }
    return hammings_method;
}