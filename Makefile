ALL:
	gcc test_set.c prime_files.c prime_calc.c -lm -o test_set.out
	gcc test_read.c prime_files.c prime_calc.c -lm -o test_read.out

#TODO: generalize the makefile