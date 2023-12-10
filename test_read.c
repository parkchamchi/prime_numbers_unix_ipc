#include <stdio.h>
#include <stdbool.h>

#include "prime_files.h"
#include "prime_calc.h"

int main(void) {
	startup();

	for (int i = 0; i < 64; i++) {
		printf("%d: ", i);

		enum Numstat res = check_num(i);
		switch (res) {
		case EMPTY:
			printf("%d: ", i);
			puts("empty");
			break;
		case PRIME:
			puts("prime");
			break;
		case NOTPRIME:
			puts("not prime");
			break;
		case ALLOC:
			puts("alloc");
			break;
		}
	}

	cleanup();
}