ALL:
	gcc test_set.c prime_files.c prime_calc.c -lm -o test_set.out
	gcc test_read.c prime_files.c prime_calc.c -lm -o test_read.out
	gcc prime_server.c prime_files.c -lm -o prime_server.out
	gcc prime_c_checknum.c -lm -o prime_c_checknum.out
	gcc prime_c_loop.c prime_calc.c -lm -o prime_c_loop.out

#TODO: generalize the makefile