#include "rsa.h"

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *KEYS_FILE = "keys.txt";
const unsigned int SIZE_OF_KEY_NAME = 11;
const unsigned int OPEN_EXP = 65537;
const size_t SIZE_OF_RAND_KEY = 1024;

gmp_randstate_t state;

void init_gmp_random() {
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
}

size_t get_approx_log2_mpz_t(mpz_t n) {
    if (mpz_sgn(n) <= 0) {
        return 0;
    }
    return mpz_sizeinbase(n, 2) - 1;
}

void get_2s_d_form_of_num(mpz_t n, mpz_t d, unsigned int *s) {
    mpz_t quotient1, remainder;
    mpz_init(quotient1);
    mpz_init(remainder);
    *s = 0;

    mpz_sub_ui(d, n, 1);
    mpz_tdiv_qr_ui(quotient1, remainder, d, 2);

    while (1) {
        if (mpz_cmp_ui(remainder, 0) > 0) {
            break;
        }
        *s += 1;
        mpz_set(d, quotient1);
        mpz_tdiv_qr_ui(quotient1, remainder, d, 2);
    }
    mpz_clear(quotient1);
    mpz_clear(remainder);
}

void powm(mpz_t res, mpz_t base, mpz_t exp, mpz_t mod) {
    mpz_t result, b, e, temp;
    mpz_init_set_ui(result, 1);
    mpz_init(b);
    mpz_init_set(e, exp);
    mpz_init(temp);

    mpz_mod(b, base, mod);

    while (mpz_cmp_ui(e, 0) > 0) {
        if (mpz_odd_p(e)) {
            mpz_mul(temp, result, b);
            mpz_mod(result, temp, mod);
        }

        mpz_mul(temp, b, b);
        mpz_mod(b, temp, mod);

        mpz_fdiv_q_2exp(e, e, 1);
    }

    mpz_set(res, result);

    mpz_clear(result);
    mpz_clear(b);
    mpz_clear(e);
    mpz_clear(temp);
}

int Miller_Rabin_test(mpz_t n) {
    mpz_t d, a, x, y;
    mpz_t temp2;
    mpz_init(d);
    mpz_init(y);
    mpz_init(x);
    mpz_init(a);

    mpz_init(temp2);
    mpz_set_ui(temp2, 2);
    unsigned int s = 0;

    get_2s_d_form_of_num(n, d, &s);
    size_t log_n = get_approx_log2_mpz_t(n);

    for (size_t i = 0; i < log_n; i++) {
        mpz_sub_ui(a, n, 4);
        mpz_urandomm(a, state, a);
        mpz_add_ui(a, a, 2); // a in [2, n - 2]
        powm(x, a, d, n);
        for (unsigned int j = 0; j < s; j++) {
            powm(y, x, temp2, n);

            if (mpz_cmp_ui(y, 1) == 0 && mpz_cmp_ui(x, 1) != 0) {
                mpz_add_ui(x, x, 1);
                if (mpz_cmp(x, n) != 0) {
                    mpz_clear(d);
                    mpz_clear(y);
                    mpz_clear(x);
                    mpz_clear(a);
                    return 0;
                }
                mpz_sub_ui(x, x, 1);
            }
            mpz_set(x, y);
        }
        if (mpz_cmp_ui(y, 1) != 0) {
            mpz_clear(d);
            mpz_clear(y);
            mpz_clear(x);
            mpz_clear(a);
            return 0;
        }
    }
    mpz_clear(d);
    mpz_clear(y);
    mpz_clear(x);
    mpz_clear(a);
    mpz_clear(temp2);
    return 1;
}

void set_random_num(mpz_t num, size_t bits) {
    do {
        mpz_urandomb(num, state, bits);
    } while (mpz_sizeinbase(num, 2) != bits);
}

void get_random_prime_num(mpz_t n) {
    while (1) {
        set_random_num(n, SIZE_OF_RAND_KEY);
        if (Miller_Rabin_test(n)) {
            break;
        }
    }
}

int mod_reverse_extended_gcd(mpz_t a, mpz_t mod, mpz_t reverse, mpz_t gcd) {
    if (mpz_sgn(mod) <= 0) {
        return 0;
    }

    mpz_t a_mod;
    mpz_init(a_mod);
    mpz_mod(a_mod, a, mod);

    mpz_t old_r, r, old_s, s, quotient, tmp;
    mpz_inits(old_r, r, old_s, s, quotient, tmp, NULL);

    mpz_set(old_r, a_mod);
    mpz_set(r, mod);
    mpz_set_ui(old_s, 1);
    mpz_set_ui(s, 0);

    while (mpz_sgn(r) != 0) {
        mpz_fdiv_q(quotient, old_r, r);
        mpz_set(tmp, r);
        mpz_mul(r, r, quotient);
        mpz_sub(r, old_r, r);
        mpz_set(old_r, tmp);

        mpz_set(tmp, s);
        mpz_mul(s, s, quotient);
        mpz_sub(s, old_s, s);
        mpz_set(old_s, tmp);
    }

    mpz_set(gcd, old_r);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        mpz_clear(a_mod);
        mpz_clears(old_r, r, old_s, s, quotient, tmp, NULL);
        return 0;
    }

    mpz_mod(reverse, old_s, mod);

    mpz_clear(a_mod);
    mpz_clears(old_r, r, old_s, s, quotient, tmp, NULL);
    return 1;
}

int try_generate_new_key_rca(mpz_t e, mpz_t n, mpz_t d) {
    mpz_t p, q, phi;
    mpz_t gcd;

    mpz_init(p);
    mpz_init(q);
    mpz_init(phi);
    mpz_init(gcd);

    get_random_prime_num(p);
    get_random_prime_num(q);

    mpz_mul(n, p, q);

    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q);

    mod_reverse_extended_gcd(e, phi, d, gcd);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        return 0;
    }

    return 1;
}

void generate_new_key_rca(mpz_t open_exp, mpz_t n, mpz_t d) {
    while (try_generate_new_key_rca(open_exp, n, d) == 0)
        ;
}

void add_key_in_file(int name) {
    mpz_t e, n, d;
    mpz_init(e);
    mpz_init(n);
    mpz_init(d);

    mpz_set_ui(e, OPEN_EXP);
    generate_new_key_rca(e, n, d);

    FILE *fp = fopen(KEYS_FILE, "a");
    char name_str[SIZE_OF_KEY_NAME];

    snprintf(name_str, sizeof(name_str), "%d", name);

    if (fp != NULL) {
        char *e_str = mpz_get_str(NULL, 10, e);
        char *n_str = mpz_get_str(NULL, 10, n);
        char *d_str = mpz_get_str(NULL, 10, d);

        fputs(name_str, fp);
        fputs(" ", fp);
        fputs(e_str, fp);
        fputs(" ", fp);
        fputs(n_str, fp);
        fputs(" ", fp);
        fputs(d_str, fp);
        fputs("\n", fp);

        free(e_str);
        free(n_str);
        free(d_str);
        fclose(fp);
    }
    mpz_clear(e);
    mpz_clear(n);
    mpz_clear(d);
}

int get_keys(int name_of_key, mpz_t e, mpz_t n, mpz_t d) {
    FILE *f = fopen(KEYS_FILE, "r");
    if (!f) {
        return -1;
    }

    char s_e[8192], s_n[8192], s_d[8192];
    int id;

    while (fscanf(f, "%d %8191s %8191s %8191s", &id, s_e, s_n, s_d) == 4) {
        if (id != name_of_key) {
            continue;
        }

        if (mpz_set_str(e, s_e, 10) != 0 || mpz_set_str(n, s_n, 10) != 0 || mpz_set_str(d, s_d, 10) != 0) {
            fclose(f);
            return -3;
        }

        fclose(f);
        return 1;
    }

    fclose(f);
    return -2;
}

void bytes_to_mpz(mpz_t out, const unsigned char *in, size_t len) {
    mpz_import(out, len, 1, 1, 1, 0, in);
}

unsigned char *mpz_to_bytes(size_t *lenp, const mpz_t in) {
    size_t count = 0;
    unsigned char *buf = (unsigned char *) mpz_export(NULL, &count, 1, 1, 1, 0, in);
    if (lenp) {
        *lenp = count;
    }
    return buf;
}

int code_message(mpz_t result, const unsigned char *message, size_t len, int key_id) {
    mpz_t e, n, d, message_mpz;
    mpz_inits(e, n, d, message_mpz, NULL);

    int ret = get_keys(key_id, e, n, d);
    if (ret != 1) {
        mpz_clears(e, n, d, message_mpz, NULL);
        printf("\nKey with id %d not found. Create it first (option 3).\n", key_id);
        return 0;
    }

    bytes_to_mpz(message_mpz, message, len);
    if (mpz_cmp(message_mpz, n) >= 0) {
        mpz_clears(e, n, d, message_mpz, NULL);
        printf("\nError: message too long for this key (max %zu bits).\n", mpz_sizeinbase(n, 2));
        return 0;
    }
    powm(result, message_mpz, e, n);

    mpz_clears(e, n, d, message_mpz, NULL);
    return 1;
}

unsigned char *decode_message(mpz_t encrypted_message, int key_id, size_t *output_len) {
    mpz_t e, n, d, message_mpz;
    mpz_inits(e, n, d, message_mpz, NULL);

    if (get_keys(key_id, e, n, d) != 1) {
        mpz_clears(e, n, d, message_mpz, NULL);
        printf("\nKey with id %d not found. Cannot decode.\n", key_id);
        return NULL;
    }

    if (mpz_cmp(encrypted_message, n) >= 0) {
        mpz_clears(e, n, d, message_mpz, NULL);
        printf("\nerror abs(m) > n)");
        return NULL;
    }
    powm(message_mpz, encrypted_message, d, n);

    size_t len;
    unsigned char *result = mpz_to_bytes(&len, message_mpz);

    if (result == NULL) {
        mpz_clears(e, n, d, message_mpz, NULL);
        fprintf(stderr, "\nerror in call mpz_to_bytes decode_message");
        return NULL;
    }

    unsigned char *new_result = realloc(result, len + 1);
    if (new_result == NULL) {
        free(result);
        mpz_clears(e, n, d, message_mpz, NULL);
        fprintf(stderr, "\nОшибка: не удалось выделить память %zu", len);
        return NULL;
    }
    result = new_result;
    result[len] = '\0';

    if (output_len) {
        *output_len = len;
    }

    mpz_clears(e, n, d, message_mpz, NULL);
    return result;
}

char *read_until_amp() {
    size_t buffer_size = 1024;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        return NULL;
    }

    size_t len = 0;
    int c;

    while ((c = getchar()) != EOF) {
        if (len == 0 && (c == '\n' || c == '\r')) {
            continue;
        }

        if (c == '&') {
            break;
        }

        if (len + 1 >= buffer_size) {
            buffer_size *= 2;
            char *new_buf = realloc(buffer, buffer_size);
            if (!new_buf) {
                free(buffer);
                return NULL;
            }
            buffer = new_buf;
        }

        buffer[len++] = (char) c;
    }

    buffer[len] = '\0';
    return buffer;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void test() {
    srandom(time(NULL));

    long num = 0;
    char num_str[25];
    unsigned char *decoded_massage;

    mpz_t codded_massage_mpz;
    mpz_init(codded_massage_mpz);

    int errors = 0;
    for (int i = 1; i < 15; i++) {
        num = random();
        add_key_in_file(i);

        snprintf(num_str, sizeof(num_str), "%ld", num);

        code_message(codded_massage_mpz, (const unsigned char *) num_str, strlen(num_str), i);

        decoded_massage = decode_message(codded_massage_mpz, i, NULL);
        printf("\n(%s) (%s)", num_str, decoded_massage);

        if (strcmp(num_str, decoded_massage) != 0) {
            errors++;
            printf(" <- MISMATCH!");
        }

        free(decoded_massage);
    }

    mpz_clear(codded_massage_mpz);

    if (errors) {
        printf("\ntest failed: %d errors", errors);
    } else {
        printf("\ntest passed");
    }
}

void app() {
    int command = 0;
    int key_id = 0;
    mpz_t coded_massage;
    mpz_init(coded_massage);
    while (1) {
        printf("\n1 - code massage; 2 - decode massage; 3 - create new key; 0 - " "exit\n");
        scanf("%d", &command);
        clear_input_buffer();
        if (command == 1) {
            printf("\nenter key_id: ");
            scanf("%d", &key_id);
            clear_input_buffer();

            printf("\nenter massage(until &): ");
            char *massage = read_until_amp();
            if (code_message(coded_massage, (unsigned char *) massage, strlen((const char *) massage), key_id)) {
                printf("\nok\n");
                printf("\ncodded massage: \n%s", mpz_get_str(NULL, 10, coded_massage));
            }
            free(massage);

        } else if (command == 2) {
            printf("\nenter key_id: ");
            scanf("%d", &key_id);
            clear_input_buffer();

            printf("\nenter massage: ");
            gmp_scanf("%Zd", coded_massage);
            clear_input_buffer();

            unsigned char *massage = decode_message(coded_massage, key_id, NULL);
            printf("\ndecoded massage: %s", massage);
            free(massage);
        } else if (command == 3) {
            printf("\nenter unsign int key id: ");
            scanf("%d", &key_id);
            add_key_in_file(key_id);

        } else if (command == 0) {
            break;
        } else {
            printf("unknown command");
        }
    }
    mpz_clear(coded_massage);
}
