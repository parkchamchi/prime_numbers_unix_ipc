#ifndef PRIME_FILES_H
#define PRIME_FILES_H

#include <stdbool.h>
#include "prime_common.h"

//Needed by the server.
void startup(void);
void cleanup(void);

primenum_t alloc_one(void); //Called by the client. The client have to calculate the given number and respond with `set_one()`.
void set_one(primenum_t target, enum Numstat stat);
enum Numstat check_num(primenum_t target); //Check if the given number is a prime number.

#endif