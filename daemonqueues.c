/*
Bidirectional queue

Message.mtype:

		1-Info
		2-Warning
		3-Error
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "daemon.h"
#include "types.h"

int initLogin(bool);
int dSrv_initLogin();
int other_initLogin();
int create_queue();
int sndMessage(char *, int);
int rcvMessage(int);
int rcv_InfoMessage();
int rcv_WarningMessage();
int rcv_ErrorMessage();
char * typetoString(long);
void printMessage();

struct Message
{
	long mtype;
	char mdata[MAXM_LEN];
	int svpid;
};

Message msg;

bool is_dSrv;

int qmessage_in, qmessage_out;

key_t k_in, k_out;


int initLogin(bool is_daemonSrv) {

	is_dSrv = is_daemonSrv;

	switch (is_dSrv) {

	case true: {
		return dSrv_initLogin();
	}
		

	case false:
		return other_initLogin();
	}
}

int dSrv_initLogin() {

	k_in = getkey("Q1");
	k_out = getkey("Q2");

	return create_queue();
}

int other_initLogin() {

	k_in = getkey("Q2");
	k_out = getkey("Q1");

	return create_queue();
}


int create_queue() {

	if ( (qmessage_in =  msgget(k_in, 0666 | IPC_CREAT)) == -1 )
		return -1;

	if ( (qmessage_out =  msgget(k_out, 0666 | IPC_CREAT)) == -1 )
		return -1;

	return 0;
}

int sndMessage(char * message, int type) {
	
	msg.mtype = type;
	msg.svpid = getpid();
	strcpy(msg.mdata, message);
	
	if (msgsnd(qmessage_out, &msg, sizeof(msg), 0)<0) {
		printf("sndMessage to %d queue failed" , qmessage_out);
		perror("Error:");
		return -1;
	}
	return 0;
}

int rcvMessage(int type)  {
	
	if (msgrcv(qmessage_in, &msg, sizeof(msg), type, 0) < 0)
	{
		printf("rcvMessage from %d queue failed", qmessage_in);
		perror("Error:"); 
		return -1;	
	}
	return 0;

}

int rcv_InfoMessage() {
	return rcvMessage(INFO_TYPE);
}

int rcv_WarningMessage() {
	return rcvMessage(WARNING_TYPE);
}

int rcv_ErrorMessage() {
	return rcvMessage(ERROR_TYPE);
}

char * typetoString(long type) {	
	char * strings[MESSAGE_TYPES] = {"Info", "Warning", "Error"};
	return strings[type-1];
}

 void printMessage() {
	char * string = (is_dSrv==0)?"Deamon":"Server";
	printf("Message from server is : %s.\nDescription : Message type %s, received from %s pid : %d\n\n", msg.mdata, typetoString(msg.mtype), string, msg.svpid);
}
