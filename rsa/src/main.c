#include "rsa.h"

int main(void) {
    init_gmp_random();
    app();
    gmp_randclear(state);
}
