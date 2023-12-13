#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "prime_common.h"
#include "prime_calc.h"

int main(int argc, char *argv[]) {
	key_t key;
	int msgid;

	key = ftok(KEYPATH, PROJID);
	msgid = msgget(key, IPC_CREAT | 0644);
	if (msgid == -1) {
		perror("msgget");
		exit(1);
	}

	int looplim = 0;
	bool infloop = true;
	if (argc >= 2) {
		infloop = false;
		looplim = atoi(argv[1]);
	}

	int i = 0;
	while (1) {
		if (!infloop && i >= looplim)
			break;

		//Alloc one
		struct primemsgbuf outmsg = { MTYP_REQ, CMD_ALLOC_ONE, { 0 } };
		if (msgsnd(msgid, (void *) &outmsg, PBUFSIZE, IPC_NOWAIT) == -1) {
			perror("msgsnd");
			exit(1);
		}

		//RECV
		struct primemsgbuf inmsg = { 0 };
		int len = msgrcv(msgid, &inmsg, PBUFSIZE, MTYP_RES_ALLOC_ONE, 0); //msgtype, msgflag

		primenum_t res = inmsg.args[0];
		printf("GOT: %lld\n", res);

		//CALC
		bool calc = is_prime(res);
		printf("%s\n", (calc) ? "prime" : "not prime");
		enum Numstat tosend = (calc) ? PRIME : NOTPRIME;

		//SEND
		struct primemsgbuf outmsg2 = { MTYP_REQ, CMD_SET_ONE, { res, tosend } };
		if (msgsnd(msgid, (void *) &outmsg2, PBUFSIZE, IPC_NOWAIT) == -1) {
			perror("msgsnd");
			exit(1);
		}
		
		i++;
	}
		
	return 0;
}