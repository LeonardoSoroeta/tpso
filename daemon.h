#ifndef _DAEMON_H
#define _DAEMON_H

#define DAEMON_CANT_ARGS 3
#define MAXM_LEN 50
#define INFO_TYPE 1
#define WARNING_TYPE 2
#define ERROR_TYPE 3
#define MESSAGE_TYPES 3

typedef struct Message Message;

int initLogin();
int sndMessage(char *, int type);
int rcvMessage(int type);
void printMessage();

#endif

