#ifndef PRIME_COMMON_H
#define PRIME_COMMON_H

typedef unsigned long long primenum_t;

#define KEYPATH "primekeyfile"
#define PROJID 97
#define MTYPE 1

#define ARGSIZE 3

struct primemsgbuf {
	long mtype;
	primenum_t args[ARGSIZE];
};

#endif