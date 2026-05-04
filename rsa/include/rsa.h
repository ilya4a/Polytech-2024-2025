#ifndef LAB1_RSA_H
#define LAB1_RSA_H
#include <gmp.h>

extern gmp_randstate_t state;

void init_gmp_random();
void add_key_in_file(int name);
int code_message(mpz_t result, const unsigned char *message, size_t len, int key_id);
unsigned char *decode_message(mpz_t encrypted_message, int key_id, size_t *output_len);
void app();

#endif // LAB1_RSA_H
