#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prime_common.h"

int main(void) {
	key_t key;
	int msgid;

	key = ftok(KEYPATH, PROJID);
	msgid = msgget(key, IPC_CREAT | 0644);
	if (msgid == -1) {
		perror("msgget");
		exit(1);
	}

	struct primemsgbuf outmsg = { MTYP_REQ, CMD_CHECK_NUM, {10, 11, 12} };
	if (msgsnd(msgid, (void *) &outmsg, sizeof (outmsg), IPC_NOWAIT) == -1) { //TODO: make a wrapper of this
		perror("msgsnd");
		exit(1);
	}

	//RECV
	struct primemsgbuf inmsg = { 0 };
	int len = msgrcv(msgid, &inmsg, sizeof (inmsg), 0, 0); //msgtype, msgflag
	printf("res: %0#llx\n", inmsg.args[0]);

	return 0;
}