#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prime_common.h"

int main(void) {
	struct primemsgbuf inmsg = { 0 };
	key_t key;
	int msgid;

	int len;

	key = ftok(KEYPATH, PROJID);
	if ((msgid = msgget(key, 0)) < 0) {
		perror("msgget");
		exit(1);
	}

	/* LOOP */
	while (1) {
		len = msgrcv(msgid, &inmsg, sizeof (inmsg), 0, 0); //msgtype, msgflag

		for (int i = 0; i < ARGSIZE; i++)
			printf("%lld ", inmsg.args[i]);
		puts("");
	}

	return 0;
}