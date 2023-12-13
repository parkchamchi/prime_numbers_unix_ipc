#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prime_common.h"

int main(int argc, char *argv[]) {
	key_t key;
	int msgid;

	key = ftok(KEYPATH, PROJID);
	msgid = msgget(key, IPC_CREAT | 0644);
	if (msgid == -1) {
		perror("msgget");
		exit(1);
	}

	int totest = 7;
	if (argc >= 2)
		totest = atoi(argv[1]);
	printf("Testing: %d\n", totest);

	struct primemsgbuf outmsg = { MTYP_REQ, CMD_CHECK_NUM, { totest } };
	if (msgsnd(msgid, (void *) &outmsg, PBUFSIZE, IPC_NOWAIT) == -1) { //TODO: make a wrapper of this
		perror("msgsnd");
		exit(1);
	}

	//RECV
	struct primemsgbuf inmsg = { 0 };
	int len = msgrcv(msgid, &inmsg, PBUFSIZE, 0, 0); //msgtype, msgflag

	enum Numstat res = (enum Numstat) inmsg.args[0];
	printf("res: %0#x\n", res);
	switch (res) {
	case EMPTY:
		puts("EMPTY");
		break;
	case PRIME:
		puts("PRIME");
		break;
	case NOTPRIME:
		puts("NOTPRIME");
		break;
	case ALLOC:
		puts("ALLOC");
		break;
	default:
		puts("ERROR: unknown res");
		break;
	}

	return 0;
}