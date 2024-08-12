#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hamming.h"
#define m 127
#define m1 120

void main() {
// Обозначение параметров:
    int length_a; // Длина слов
    int i; // Номер слова в сообщении
    int k, k1, k2; // Счетчики
    int count; // Количество символов "1" в слове
    int syndrom; // Определяет место ошибки в слове 
    char c[m]; // Символ, заменяемый "0" в слове
    char c1[m1]; // Символ, сдвигающий слово

// 1 этап - ввод данных
    printf("Length of message: \n"); // Длина сообщения
    scanf("%d", &length_a); getchar(); // Количество слов в сообщении
    char array[length_a][m1]; // Массив-сообщение, состоящее из слов, состоящее из символов
    printf("Enter %d words:\n", length_a); 
    for (i = 0; i < length_a; i++) {
        fgets(array[i], m1, stdin); // Сообщение
        array[i][strcspn(array[i], "\n")] = '\0'; // Длина сообщения
    }
//2 этап - определение метода изменения сообщения
    printf("Select a method: \nTo encrypt a message, press 1. \nTo decrypt a message, press 2. \n");
    scanf("%d",&i); //Действие: 1 - шифрование по методу Хамминга; 2 - дешифрование по методу Хамминга

 //3.1 этап - шифрование сообщения
    //а) вставка "0" на месте двоичного символа с со сдвигом слова
    if (i==1) {
        for (i = 0; i < length_a; i++) {
            for (k1 = 1; k1 <= strlen(array[i]); k1 <<= 1) {
                c[k1 - 1] = array[i][k1 - 1];
                array[i][k1 - 1] = '0';
                for (k2 = k1; k2 <= strlen(array[i]); k2++) {
                    c1[k2] = array[i][k2];
                    array[i][k2] = c[k2 - 1];
                    c[k2] = c1[k2];
                } 
            }

    //б) подстановка "1" за место вставленного "0", если XOR = 1
            for (k = 1; k <= strlen(array[i]); k <<= 1)  {
                for (k1 = k - 1, count = 0; k1 <= strlen(array[i]); k1 = k1 + (2 * k)) {
                    for (k2 = k1; k2 <= k1 + k - 1 && k2 <= strlen(array[i]); k2++) {
                        if(array[i][k2] == '1') {
                            count = count + 1;
                        }
                    }  
                }
                if (count % 2 == 1) {
                    array[i][k - 1] = '1';
                }
            }
        }
        printf("Encoded message: "); //Зашифрованное сообщение
    }

//3.2 этап - дешифрование сообщения 
    //а) определение типа возможной ошибки и подстановка единицы на нужное место
    else {
        for (i = 0; i < length_a; i++) {
            for (k = 1, syndrom = 0; k <= strlen(array[i]); k <<= 1) {
                for (k1 = k - 1, count = 0; k1 < strlen(array[i]); k1 = k1 + (2 * k)) {
                    for (k2 = k1; k2 <= k1 + k - 1 && k2 < strlen(array[i]); k2++) {
                        if(array[i][k2] == '1') {
                            count = count + 1;
                        }
                    }  
                }
                if (count % 2 == 1) {
                    syndrom += (1 * k);
                }
            }
            if (syndrom != 0 && syndrom > strlen(array[i]) || (strlen(array[i]) && !(strlen(array[i]) & (strlen(array[i])) - 1))) { // При двойной ошибке
                printf("Double error! Cannot apply decoder to this message: %s.\n", array[i]); 
                for (k = i; k < length_a - 1; k++) {
                    strcpy(array[k], array[k + 1]); // Убирает ошибочное слово
                }
            length_a--;
            i--;
            }
            if (syndrom != 0 && syndrom <= strlen(array[i])) { // При единичной ошибке
                printf("One error in: %s. ", array[i]);
                printf("The word was corrected on the %d character. ", syndrom);
                if (array[i][syndrom - 1] == '0') { //Заменяет символ на противоположный
                    array[i][syndrom-1] = '1';
                }
                else {
                    array[i][syndrom - 1] = '0';
                }
                printf("New word is: %s.\n", array[i]);
            }
        }

    //б) убирание символа на степенном месте с со сдвигом слова
        for (i = 0; i < length_a; i++) {
            for (k1 = 1, k2 = 1; k1 <= strlen(array[i]); k1 <<= 1, k2++) {
                memmove(&array[i][k1 - k2], &array[i][k1 - k2 + 1], strlen(array[i]));
            }
        }
        printf("Encrypted message: "); //Дешифрованное слово
    }


// Передача массива в функцию
    char** area = hamming(array, length_a); //Из hamming.c
    for (i = 0; i < length_a; i++) {
        printf("%s ", area[i]); // Вывод сообщения
        free(area[i]); // Освобождение памяти для каждого слова
    }
    free(area); // Освобождение памяти для символов
    return 0;
}