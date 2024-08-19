//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Made by APTEM 3OTOB %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
#include "xtea.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define m 64

char** xtea(uint32_t hex_array[][2], int length) {
    char** xtea_method = (char**)malloc(length * sizeof(char*));
    for (int i = 0; i < length; i++) {
        xtea_method[i] = (char*)malloc(m * sizeof(char));
        sprintf(xtea_method[i], "%08X%08X", hex_array[i][0], hex_array[i][1]); 
    }
    return xtea_method;
}