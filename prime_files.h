#ifndef PRIME_FILES_H
#define PRIME_FILES_H

#include <stdbool.h>
#include "prime_common.h"

enum Numstat { EMPTY = 00, PRIME = 01, NOTPRIME = 02, ALLOC = 03 };

void startup(void);
void cleanup(void);

primenum_t alloc_one(void);
void set_one(primenum_t target, bool is_prime);
enum Numstat check_num(primenum_t target);

#endif