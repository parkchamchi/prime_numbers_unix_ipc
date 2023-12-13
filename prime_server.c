#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prime_common.h"
#include "prime_files.h"

void signal_handler(int signal) {
		if (signal == SIGINT) {
        	printf("\nCtrl+C pressed. Cleaning up...\n");
        	cleanup();
        	exit(0);
    	}
}


int main(void) {
		struct primemsgbuf inmsg = { 0 };
		key_t key;
		int msgid;

		int len;

		key = ftok(KEYPATH, PROJID);
		if ((msgid = msgget(key, IPC_CREAT | 0644)) < 0) {
			perror("msgget");
			exit(1);
		}

		startup();

	/* LOOP */
		while (1) {
			len = msgrcv(msgid, &inmsg, PBUFSIZE, 0, 0); //msgtype, msgflag

			for (int i = 0; i < ARGSIZE; i++)
				printf("%lld ", inmsg.args[i]);
			puts("");

			if (inmsg.cmd == CMD_CHECK_NUM) {
				puts("CHECK_NUM");
				enum Numstat res = check_num(inmsg.args[0]);

			//ret.
				struct primemsgbuf outmsg = { MTYP_RES, inmsg.cmd, { res } };
				if (msgsnd(msgid, (void *) &outmsg, PBUFSIZE, IPC_NOWAIT) == -1) {
					perror("msgsnd");
					exit(1);
				}
			}
		}

		cleanup(); //TODO: when interrupted or killed, make sure this is activated using signal catching
		return 0;
}