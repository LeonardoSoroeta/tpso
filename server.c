#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "types.h"
#include "comm.h"
#include "daemon.h"
#include "constants.h"
#include "semaphores.h"

void srv_sigRutine(int);
void initDB_calls();
void newSession(Connection *);
void server_close();
void server_process_data();
void communicate_with_database();

char * getaddress();

int session_ended = 0;

int semaphore_id;

Data * data_from_client;
Data * data_to_send;

char character[100];
int totalExp;

Listener * listener;
Connection * connection;

int main(int argc, char *argv[]) {

    char * address = "/tmp/listener";//getaddress();

    semaphore_id = binary_semaphore_allocation (666, IPC_RMID);

	listener = comm_listen(address);

	printf("[server] awaiting connection requests\n");

	while(1) {

		connection = comm_accept(listener);

		int newpid = fork();

		if(newpid == 0) {

			newSession(connection);

			return 0;
		
		}

	}

}

void newSession(Connection * connection) {

	printf("[session %d] new client session started\n", getpid());

	while (1) {

		data_from_client = receiveData(connection);

		server_process_data();

		sendData(connection, data_to_send);

		if(session_ended) {

			comm_disconnect(connection);

			return ;

		}

	}

}

void server_process_data() {

	if(data_from_client->opcode == SELECT_CHARACTER) {

		communicate_with_database();

	} else if(data_from_client->opcode == CREATE_CHARACTER) {

		communicate_with_database();

	} else if(data_from_client->opcode == EXP_UP) {

		communicate_with_database();

	} else if(data_from_client->opcode == EXIT) {

		printf("[session %d] session ended\n", getpid());

		session_ended = 1;

	} else if(data_from_client->opcode == EXIT_AND_LOGOUT) {

		printf("[session %d] session ended\n", getpid());

		communicate_with_database();

		session_ended = 1;

	} else {

		printf("[session %d] error: unknown operation requested from client\n", getpid());

	}

}

void communicate_with_database() {

	Connection * dataBaseConnection;

	binary_semaphore_wait(semaphore_id);

	dataBaseConnection = comm_connect("/tmp/database_channel");

	sendData(dataBaseConnection, data_from_client);

	data_to_send = receiveData(dataBaseConnection);

	comm_disconnect(dataBaseConnection);

	binary_semaphore_post(semaphore_id);

}
