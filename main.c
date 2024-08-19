//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Made by APTEM 3OTOB %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "hamming.h"
#include "xtea.h"
#define m 64
#define m1 70
#define round 32 // Количество раундов / параметр меняемый
#define delta 0x9E3779B9 // сумма всех комбинаций в шестнадцатеричной системе, DELTA = 2^32

// Обозначение параметров:
void main() {
    int length_a; // Длина слов для метода Хамминга
    int length; // Длина слов для метода XTEA
    int i; // Номер слова в сообщении
    int k, k1, k2; // Счетчики
    int count; // Количество символов "1" в слове
    int syndrom; // Определяет место ошибки в слове 
    char c[m]; // Символ, заменяемый "0" в слове
    char c1[m1]; // Символ, сдвигающий слово
    uint32_t key[4] = {0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210}; // Ключи для XTEA
    uint32_t sum; // Сумма всех раундов, sum = 0x00000000;

// 1 этап - подготовка к шифрованию
    //a) ввод данных
    printf("Length of message: \n"); // Длина сообщения
    scanf("%d", &length_a); getchar(); // Количество слов в сообщении
    char bin_array[length_a][m1]; // Массив-сообщение, состоящее из слов, состоящее из символов
    printf("Enter %d words:\n", length_a); 
    for (i = 0; i < length_a; i++) {
        fgets(bin_array[i], m1, stdin); // Ввод двоичного сообщения ("0" и "1")
        bin_array[i][strcspn(bin_array[i], "\n")] = '\0'; // Длина сообщения
    }
    length = length_a;
    //б) преобразование в шестнадцатеричное сообщение
    uint32_t hex_array[length_a][2]; // Массив шифра XTEA, переведенный в шестнадцатеричную систему
    for (i = 0; i < length_a; i++) {
        hex_array[i][0] = hex_array[i][1] = 0;
        if (strlen(bin_array[i]) < round) {
            for (k = 0; k < strlen(bin_array[i]); k++) {
                hex_array[i][1] = (hex_array[i][1] << 1) + (bin_array[i][k] - '0');         
            }
        }
        else {
            for (k = 0; k < strlen(bin_array[i]) - round; k++) {
                hex_array[i][0] = (hex_array[i][0] << 1) + (bin_array[i][k] - '0'); 
            }
            for (k = (strlen(bin_array[i]) - round); k < strlen(bin_array[i]); k++) {
                hex_array[i][1] = (hex_array[i][1] << 1) + (bin_array[i][k] - '0');
            }
        }
    }
    //в) вывод и проверка полученных данных
    printf("Word in binary system: "); //Слово в бинарной системе
    for (i = 0; i < length_a; i++) {
        printf("%s ", bin_array[i]); 
    }
    printf("\nWord in hexadecimal system: "); //Слово в шестнадцатеричной системе
    for (i = 0; i < length_a; i++) {
        printf("%08X%08X ", hex_array[i][0], hex_array[i][1]); 
    }

//2 этап - определение метода и преобразование сообщения
    printf("\nSelect a method: \nTo encrypt a message, press 1. \nTo decrypt a message, press 2. \n");
    scanf("%d",&i); //Действие: 1 - шифрование по методу Хамминга; 2 - дешифрование по методу Хамминга
    //а) зашифровка сообщения
    if (i==1) {
    //Hamming
        for (i = 0; i < length_a; i++) {
            for (k1 = 1; k1 <= strlen(bin_array[i]); k1 <<= 1) {
                c[k1 - 1] = bin_array[i][k1 - 1];
                bin_array[i][k1 - 1] = '0';
                for (k2 = k1; k2 <= strlen(bin_array[i]); k2++) {
                    c1[k2] = bin_array[i][k2];
                    bin_array[i][k2] = c[k2 - 1];
                    c[k2] = c1[k2];
                } 
            }
            for (k = 1; k <= strlen(bin_array[i]); k <<= 1)  {
                for (k1 = k - 1, count = 0; k1 <= strlen(bin_array[i]); k1 = k1 + (2 * k)) {
                    for (k2 = k1; k2 <= k1 + k - 1 && k2 <= strlen(bin_array[i]); k2++) {
                        if(bin_array[i][k2] == '1') {
                            count = count + 1;
                        }
                    }  
                }
                if (count % 2 == 1) {
                    bin_array[i][k - 1] = '1';
                }
            }
        }
        //XTEA
        for (i = 0; i < length; i++) { 
            for (k = 0, sum = 0; k < round; k++){ 
                hex_array[i][0] += (((hex_array[i][1] << 4) ^ (hex_array[i][1] >> 5)) + hex_array[i][1]) ^ (sum + key[sum & 3]);
                sum += delta;
                hex_array[i][1] += (((hex_array[i][0] << 4) ^ (hex_array[i][0] >> 5)) + hex_array[i][0]) ^ (sum + key[(sum >> 11) & 3]);
            }
        }
        printf("Encrypted message:\n");
    }
    //б) дешифровка сообщения
    else {
        //Hamming
        for (i = 0; i < length_a; i++) {
            for (k = 1, syndrom = 0; k <= strlen(bin_array[i]); k <<= 1) {
                for (k1 = k - 1, count = 0; k1 < strlen(bin_array[i]); k1 = k1 + (2 * k)) {
                    for (k2 = k1; k2 <= k1 + k - 1 && k2 < strlen(bin_array[i]); k2++) {
                        if(bin_array[i][k2] == '1') {
                            count = count + 1;
                        }
                    }  
                }
                if (count % 2 == 1) {
                    syndrom += (1 * k);
                }
            }
            if (syndrom != 0 && syndrom > strlen(bin_array[i]) || (strlen(bin_array[i]) && !(strlen(bin_array[i]) & (strlen(bin_array[i])) - 1))) { // При двойной ошибке
                printf("Double error! Cannot apply decoder to this message: %s.\n", bin_array[i]); 
                for (k = i; k < length_a - 1; k++) {
                    strcpy(bin_array[k], bin_array[k + 1]); 
                }
            length_a--;
            i--;
            }
            if (syndrom != 0 && syndrom <= strlen(bin_array[i]) && (strlen(bin_array[i]) && !(strlen(bin_array[i]) & (strlen(bin_array[i]))- 1))) { // При единичной ошибке
                printf("One error in: %s. ", bin_array[i]);
                printf("The word was corrected on the %d character. ", syndrom);
                if (bin_array[i][syndrom - 1] == '0') { 
                    bin_array[i][syndrom-1] = '1';
                }
                else {
                    bin_array[i][syndrom - 1] = '0';
                }
                printf("New word is: %s.\n", bin_array[i]);
            }
        }
        for (i = 0; i < length_a; i++) {
            for (k1 = 1, k2 = 1; k1 <= strlen(bin_array[i]); k1 <<= 1, k2++) {
                memmove(&bin_array[i][k1 - k2], &bin_array[i][k1 - k2 + 1], strlen(bin_array[i]));
            }
        }
        //XTEA
        for (i = 0; i < length; i++) {
            for (k = 0, sum = round * delta; k < round; k++){
                hex_array[i][1] -= (((hex_array[i][0] << 4) ^ (hex_array[i][0] >> 5)) + hex_array[i][0]) ^ (sum + key[(sum >> 11) & 3]);
                sum -= delta; 
                hex_array[i][0] -= (((hex_array[i][1] << 4) ^ (hex_array[i][1] >> 5)) + hex_array[i][1]) ^ (sum + key[sum & 3]);
            }
        }
        printf("Decrypted message:\n");
    }

//3 этап - передача массива в функцию
    //а) вывод сообщения по методу Хамминга
    printf("Hamming's method: ");
    char** hammings_method = hamming(bin_array, length_a); //Из hamming.c
    for (i = 0; i < length_a; i++) {
        printf("%s ", hammings_method[i]); //Вывод сообщения по методу Хамминга
        free(hammings_method[i]); 
    }
    free(hammings_method); 
    //б) вывод сообщения по методу XTEA
    printf("\nXTEA method: ");
    char** xtea_method = xtea(hex_array, length); //Из xtea.c
    for (i = 0; i < length; i++) {
        printf("%08X%08X ", hex_array[i][0], hex_array[i][1]); //Вывод сообщения по методу XTEA
        free(xtea_method[i]);
    }
    free(xtea_method); 
    return 0;
}