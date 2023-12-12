#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prime_common.h"

int main(void) {
	key_t key;
	int msgid;
	struct primemsgbuf msg = { 0 };

	key = ftok(KEYPATH, PROJID);
	msgid = msgget(key, IPC_CREAT | 0644);
	if (msgid == -1) {
		perror("msgget");
		exit(1);
	}

	msg.mtype = MTYPE;
	for (int i = 0; i < ARGSIZE; i++)
		msg.args[i] = i + 10;

	if (msgsnd(msgid, (void *) &msg, sizeof (msg), IPC_NOWAIT) == -1) {
		perror("msgsnd");
		exit(1);
	}

	return 0;
}