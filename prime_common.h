#ifndef PRIME_COMMON_H
#define PRIME_COMMON_H

typedef unsigned long long primenum_t;

#define KEYPATH "primekeyfile"
#define PROJID 97

#define ARGSIZE 3

enum PrimeCommands { CMD_ALLOC_ONE, CMD_SET_ONE, CMD_CHECK_NUM };
enum PrimeMtypes { MTYP_REQ = 1, MTYP_RES };

struct primemsgbuf {
	long mtype; //Can we use this as PID identifier?
	enum PrimeCommands cmd;
	primenum_t args[ARGSIZE];
};

#endif