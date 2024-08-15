#include <stdio.h>
#include <stdint.h>

#define ROUNDS 32
#define DELTA 0x9E3779B9

void xtea_encipher(unsigned int num_rounds, uint32_t *v, uint32_t const *k) {
    unsigned int i;
    uint32_t v0 = v[0], v1 = v[1], sum = 0, delta = 0x9E3779B9;
    for (i = 0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
    }
    v[0] = v0;
    v[1] = v1;
}

void xtea_decipher(unsigned int num_rounds, uint32_t *v, uint32_t const *k) {
    unsigned int i;
    uint32_t v0 = v[0], v1 = v[1], delta = 0x9E3779B9, sum = delta * num_rounds;
    for (i = 0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
    }
    v[0] = v0;
    v[1] = v1;
}

int main() {
    uint32_t key[4] = {0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210};
    uint32_t data[2] = {0x12345678, 0x9ABCDEF0};
    unsigned int num_rounds = 32;

    printf("Original Data: %08X %08X\n", data[0], data[1]);

    // Шифрование
    xtea_encipher(num_rounds, data, key);
    printf("Encrypted Data: %08X %08X\n", data[0], data[1]);

    // Дешифрование
    xtea_decipher(num_rounds, data, key);
    printf("Decrypted Data: %08X %08X\n", data[0], data[1]);

    return 0;
}