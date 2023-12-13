#ifndef PRIME_COMMON_H
#define PRIME_COMMON_H

typedef unsigned long long primenum_t;

#define KEYPATH "primekeyfile"
#define PROJID 97

#define ARGSIZE 3
#define PBUFSIZE (sizeof (struct primemsgbuf) - sizeof (long))

//EMPTY: "not set, unknown", ALLOC: "allocated by other process, or unavailable"
enum Numstat { EMPTY = 00, PRIME = 01, NOTPRIME = 02, ALLOC = 03 };

enum PrimeCommands { CMD_ALLOC_ONE = 1, CMD_SET_ONE, CMD_CHECK_NUM };
enum PrimeMtypes { MTYP_REQ = 123, MTYP_RES_ALLOC_ONE, MTYP_RES_CHECK_NUM };

struct primemsgbuf {
	long mtype; //Can we use this as PID identifier?
	enum PrimeCommands cmd;
	primenum_t args[ARGSIZE];
};

#endif