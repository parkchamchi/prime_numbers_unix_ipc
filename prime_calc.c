#include <stdbool.h>

#include "prime_calc.h"

bool is_prime(primenum_t n) {
		if (n == 2 || n == 3)
			return true;

		if (n <= 1 || n % 2 == 0 || n % 3 == 0)
			return false;

		for (int i = 5; i*i <= n; i += 6) { //5, 11, 17, 23, ...
			if (n % i == 0 || n % (i+2) == 0)
				return false;
		}

		return true;
}