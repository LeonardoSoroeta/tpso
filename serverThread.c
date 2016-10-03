#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "types.h"
#include "comm.h"
#include "daemon.h"
#include "states.h"

void srv_sigRutine(int);
void initDB_calls();
void * newSession(void *);
void server_close();
void current_close(Connection *);

char * getadress();
Data * receiveData(Connection *);

Listener * listener;
Connection * connection;

int session;

pthread_t thread_id[MAX_THREADS];

int main(int argc, char *argv[])
{
	signal(SIGINT, srv_sigRutine);

	if ( initLogin(false) == -1 ) {
    	printf("Couldn´t create message queue for server\n");
    	exit(1);
    }
    
    sndMessage("Server is logged in", INFO_TYPE); // sends message to daemonServer
    
    initDB_calls();

    char * address = getadress();

	listener = comm_listen(address);

	if (listener==NULL) {
		sndMessage("Couldn´t create listener", ERROR_TYPE);
		exit(1);
	}

	printf("[server] awaiting connection requests\n");

	session=0;

	int i =0;

	while(1) {

		connection = comm_accept(listener);

		if (connection==NULL) {
			sndMessage("Couldn´t create connection from listener", ERROR_TYPE);
			exit(1);
		}
		
		pthread_create(&thread_id[i++], NULL ,&newSession, connection );	
		
	}
}

void * newSession(void * args) {

	Connection * current_connection = (Connection *) args;

	int current_session = ++session;

	Data * data_from_client;
	Data * data_to_send;

	printf("[session %d] new client session started\n", current_session);

	while (1) {

		data_from_client = receiveData(current_connection);

		if (data_from_client == NULL) {
			exit(1);
		}

		if(data_from_client->opcode == END_OF_CONNECTION) {

			printf("[session %d] session ended, END_OF_CONNECTION opcode received\n", current_session);

			sndMessage("Server is logged out with no errors", INFO_TYPE);

			current_close(current_connection);
			
			return;

		}  else if(data_from_client->opcode == CONNECTION_INTERRUMPED) {

			printf("[session %d] session ended, CONNECTION_INTERRUMPED opcode received\n", current_session);

			sndMessage("Server is logged out by kill on client", WARNING_TYPE);

			current_close(current_connection);

			return;

		} else if(data_from_client->opcode == TEST_MESSAGE_STRING) {

			printf("[session %d] received message: %s", current_session, data_from_client->avmdata.message);

		} else {

			printf("[session %d] error: unknown operation requested\n", current_session);

		}
	}
}

void initDB_calls() {

}

void srv_sigRutine(int sig) {

    printf("\n");
    printf("Server proccess with pid: %d terminated\n", getpid());

    sndMessage("Server logged out by kill()", WARNING_TYPE);
    
	free(listener);

    exit(1);
}

void server_close() {
	
}

void current_close(Connection * current_connection) {
	
	comm_disconnect(current_connection);
	free(current_connection);
}