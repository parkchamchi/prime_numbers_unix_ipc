CC = gcc
CFLAGS = 
LFLAGS = -lm

all: prime_server.out prime_c_checknum.out prime_c_loop.out

prime_server.out: prime_server.c prime_files.c
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o $@

prime_c_checknum.out: prime_c_checknum.c
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o $@

prime_c_loop.out: prime_c_loop.c prime_calc.c
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o $@

clean:
	rm -f prime_server.out prime_c_checknum.out prime_c_loop.out