#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prime_common.h"
#include "prime_files.h"

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

	startup();

	/* LOOP */
	while (1) {
		len = msgrcv(msgid, &inmsg, sizeof (inmsg), 0, 0); //msgtype, msgflag

		for (int i = 0; i < ARGSIZE; i++)
			printf("%lld ", inmsg.args[i]);
		puts("");

		if (inmsg.cmd == CMD_CHECK_NUM) {
			puts("CHECK_NUM");
			enum Numstat res = check_num(inmsg.args[0]);

			//ret.
			struct primemsgbuf outmsg = { MTYP_RES, inmsg.cmd, { res } };
			if (msgsnd(msgid, (void *) &outmsg, sizeof (outmsg), IPC_NOWAIT) == -1) {
				perror("msgsnd");
				exit(1);
			}
		}
	}

	cleanup(); //TODO: when interrupted or killed, make sure this is activated using signal catching
	return 0;
}