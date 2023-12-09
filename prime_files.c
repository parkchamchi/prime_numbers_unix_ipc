#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "prime_files.h"

//#define DEBUG
#ifdef DEBUG
	#define PRINTLOG printf
#else
	#define PRINTLOG dummyprintf
#endif

#define MAX_WORKING_BIT 31
#define WORKINGFILE_EXT "bin"
#define OUTFILE_EXT "primes"
#define OUTDIR "outfiles/"
#define FILENAME_BUFSIZE 64
#define WORDSIZE (sizeof (int))

typedef int file_handler;
static file_handler fh_g = 0;
static int current_working_bit = 0;

static void get_out_filename(char buf[], int n);
static void get_working_filename(char buf[], int n);

//Get the current "working bit": that is, the bit that is fixed.
//e.g., when the working bit is 3, the numbers that have to be calculated is in 0b0...01xxx
//Returns a negative integer when none is available
static int get_current_working_bit(void);

static bool file_exists(const char *path);
static file_handler open_file(const char *path);
static void close_file(file_handler fh);
static void init_file(file_handler fh, int n);
static primenum_t next_empty(void);
static void increment_working_number(void);

static int dummyprintf(const char *, ...);

void startup(void) {
	char filename[FILENAME_BUFSIZE] = { 0 };

	//Does the dir exist?
	if (!file_exists(OUTDIR))
		mkdir(OUTDIR, 0777);

	bool should_check = (current_working_bit == 7);
	PRINTLOG("working bit is 7.\n");

	//Get the current working bit
	int n = get_current_working_bit();

	PRINTLOG("new working bit is: %d\n", n);

	//Does the current workingfile exist?
	get_working_filename(filename, n);
	bool current_workingfile_exists = file_exists(filename);

	PRINTLOG("Opening %s\n", filename);
	file_handler fh = open_file(filename);
	fh_g = fh;
	current_working_bit = n;

	PRINTLOG("%d\n", current_workingfile_exists);
	if (!current_workingfile_exists)
		init_file(fh, n);
}

void cleanup(void) {
	close_file(fh_g);
}

primenum_t alloc_one(void) {
	return next_empty();
}

void set_one(primenum_t target, bool is_prime) {
	file_handler fh = fh_g;
	char c = 0;
	
	PRINTLOG("set_one, %lld\n", target);

	target -= pow(2, current_working_bit);
	int loc = target / 4;

	PRINTLOG("set_one2, %lld\n", target);

	PRINTLOG("loc: %d\n", loc);

	lseek(fh, loc, SEEK_SET);
	read(fh, &c, 1);

	PRINTLOG("original c: %#x\n", c);

	enum Numstat stat = (is_prime) ? PRIME : NOTPRIME;
	c |= (stat) << ((3 - target % 4) * 2);

	PRINTLOG("new c: %#x\n", c);
	
	lseek(fh, loc, SEEK_SET);
	if (write(fh, &c, 1) == -1)
		perror("write");
}

enum Numstat check_num(primenum_t target) {
	if (target < 2) return NOTPRIME;
	
	//Check the biggest bit position
	int pos = 1;
	primenum_t test = target;
	for (int i = 0; i < MAX_WORKING_BIT+1; i++) {
		int out = test & 0x1;
		if (out)
			pos = i;

		test >>= 1;
	}

	//out-of-bound
	if (pos > MAX_WORKING_BIT)
		return ALLOC;
	
	//Check if the target file exists
	file_handler fd = -1;
	bool should_close_fd = false;
	if (pos == current_working_bit) {
		fd = fh_g;
		should_close_fd = false;
	}
	else {
		char targetfile[FILENAME_BUFSIZE] = { 0 };
		get_out_filename(targetfile, pos);
		
		if (!file_exists(targetfile))
			return ALLOC;

		fd = open_file(targetfile);
		should_close_fd = true;
	}

	//Read
	target -= pow(2, pos);
	int loc = target / 4;
	char c = 0;

	lseek(fd, loc, SEEK_SET);
	read(fd, &c, 1);

	c >>= ((3 - target % 4) * 2);
	c &= 0x3;
	
	if (should_close_fd)
		close_file(fd);

	return c;
}

static void get_out_filename(char buf[], int n) {
	sprintf(buf, "%s%d.%s", OUTDIR, n, OUTFILE_EXT);
}

static void get_working_filename(char buf[], int n) {
	sprintf(buf, "%s%d.%s", OUTDIR, n, WORKINGFILE_EXT);	
}

static int get_current_working_bit(void) {
	char filename[FILENAME_BUFSIZE] = { 0 };

	for (int i = 1; i <= MAX_WORKING_BIT; i++) {
		get_out_filename(filename, i);
		PRINTLOG("Searching %s\n", filename);
		if (file_exists(filename)) {
			PRINTLOG("found");
		}
		else {
			PRINTLOG("not found");
			return i;
		}
	}

	return -1;
}

static bool file_exists(const char *path) {
	return access(path, F_OK) == 0;
}

static file_handler open_file(const char *path) {
	int fd = open(path, O_RDWR | O_CREAT, 0777);
	return fd;
}

static void close_file(file_handler fh) {
	close(fh);
}

static void init_file(file_handler fh, int n) {
	//Fill with 0's, len == n*2 bits

	char buf[WORDSIZE] = { 0 };
	assert(EMPTY == 0);

	//long long len = pow(2, n); //need 2**n entries...
	//len *= 2; //2 bits per entry...
	//len /= 8; //bytes...
	//len /= WORDSIZE; //write by WORDSIZE...
	int len = pow(2, n-2 - 2);
	len = fmax(len, 4);

	//need 2**7 entries, that is, 2 ** 7 * 2 bits, that is, 2^8 bits.
	if (n == 7) {
		PRINTLOG("n == 7.\n");
		PRINTLOG("writing %ld bytes.\n", len * WORDSIZE);
	}

	PRINTLOG("Init. file\n");
	for (int i = 0; i < len; i++) {
		if (write(fh, buf, WORDSIZE) != WORDSIZE)
			perror("write on init");
	}
}

static primenum_t next_empty(void) {
	primenum_t len = pow(2, current_working_bit);
	void *buf = calloc(1, 1);
	primenum_t res = 0;
	bool got_one = false;

	lseek(fh_g, 0, SEEK_SET);
	primenum_t i = 0;
	do {
		//Read the entry
		read(fh_g, buf, 1);

		for (int j = 0; j < 4; j++) {
			if (i > len) break;
			PRINTLOG("%lld, %lld\n", i, len);

			int roi = *((int *) buf);
			roi >>= (3 - j) * 2;

			PRINTLOG("roi: %#x\n", roi);

			roi &= 0x3;

			PRINTLOG("roi: %#x\n", roi);

			if (roi == EMPTY) {
				res = i;
				got_one = true;
				PRINTLOG("ok, got one...: %lld, %lld\n", i, len+i);
				break;
			}

			i++;
		}
	} while (i < len && !got_one);

	free(buf);

	if (!got_one || res >= len) {
		PRINTLOG("need new one");
		increment_working_number();
		return next_empty();
	}
	else if (res < len) {
		PRINTLOG("ret. %lld + %lld = %lld\n", len, res, len + res);
		return len + res;		
	}
}

static void increment_working_number(void) {
	close_file(fh_g);

	//Transform from working... to out...
	char oldname[FILENAME_BUFSIZE] = { 0 };
	char newname[FILENAME_BUFSIZE] = { 0 };

	get_working_filename(oldname, current_working_bit);
	get_out_filename(newname, current_working_bit);
	rename(oldname, newname);

	current_working_bit++;
	fh_g = 0;

	if (current_working_bit <= MAX_WORKING_BIT)
		startup();
	else
		PRINTLOG("full.");
}

static int dummyprintf(const char *s, ...) { (void) s; return 0; }