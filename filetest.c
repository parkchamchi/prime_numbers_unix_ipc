#include <stdio.h>
#include <stdbool.h>

#include "prime_files.h"
#include "prime_calc.h"

int main(void) {
	startup();

/*
	for (int i = 0; ; i++) {
		primenum_t next = alloc_one();
		bool res = is_prime(next);
		//bool res = true;

		if (res)
			printf("*******next: %lld: %s\n", next, (res) ? "yes" : "no");

		set_one(next, res);
	}
*/	

	for (int i = 0; i < 256; i++) {
		//printf("%d: ", i);

		enum Numstat res = check_num(i);
		switch (res) {
		case EMPTY:
			printf("%d: ", i);
			puts("empty");
			break;
		case PRIME:
			//puts("prime");
			break;
		case NOTPRIME:
			//puts("not prime");
			break;
		case ALLOC:
			//puts("alloc");
			break;
		}
	}
	

	cleanup();
}