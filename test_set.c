#include <stdio.h>
#include <stdbool.h>

#include "prime_files.h"
#include "prime_calc.h"

int main(void) {
	startup();

	for (int i = 0; ; i++) {
		primenum_t next = alloc_one();
		bool res = is_prime(next);
		//bool res = true;

		if (res)
			printf("*******next: %lld: %s\n", next, (res) ? "yes" : "no");

		set_one(next, res);
	}

	cleanup();
}