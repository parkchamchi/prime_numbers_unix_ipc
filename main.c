#include <stdio.h>
#include <stdbool.h>

#include "prime_calc.h"

int main(void) {
	primenum_t n = 0;

	for (n = 0; ; n++) {
		bool res = is_prime(n);

		if (res)
			printf("%lld\n", n);
	}

	return 0;
}