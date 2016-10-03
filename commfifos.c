#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "types.h"
#include "comm.h"

struct Connection {

    int incoming_fd;
    int outgoing_fd;

};

struct Listener {

    char address[32];

};

typedef struct {

    int requester_pid;

} ConnectionRequest;

Connection * comm_connect(char * address) {

    int listener_fd;
    char client_incoming[32];
    char client_outgoing[32];
    ConnectionRequest connectionRequest;
    Connection * connection;

    connectionRequest.requester_pid = getpid();

    sprintf(client_incoming, "/tmp/client_%d_incoming", connectionRequest.requester_pid);
    sprintf(client_outgoing, "/tmp/client_%d_outgoing", connectionRequest.requester_pid);

    unlink(client_incoming);
    unlink(client_outgoing);

    if (mknod(client_incoming, S_IFIFO | 0666, 0) == -1) {
      perror("Couldn´t create client incoming named pipe. Error");
      return NULL;
    }
    
    if (mknod(client_outgoing, S_IFIFO | 0666, 0) == -1) {
      perror("Couldn´t create client outgoing named pipe. Error");
      return NULL;
    }

    listener_fd = open(address, O_WRONLY);

    write(listener_fd, &connectionRequest, sizeof(ConnectionRequest));

    close(listener_fd);

    connection = malloc(sizeof(Connection));

    connection->incoming_fd = open(client_incoming, O_RDONLY);
    connection->outgoing_fd = open(client_outgoing, O_WRONLY);

    return connection;

}

Listener * comm_listen(char * address) {

    Listener * listener;

    listener = malloc(sizeof(Listener));

    unlink(address);

    if (mknod(address, S_IFIFO | 0666, 0) == -1) {
      perror("Couldn´t create server private named pipe. Error");
      return NULL;
    }

    strcpy(listener->address, address);

    return listener;
}

Connection * comm_accept(Listener * listener) {

    int listener_fd;
    char client_incoming[32];
    char client_outgoing[32];
    ConnectionRequest connectionRequest;
    Connection * connection;

    connection = malloc(sizeof(Connection));

    listener_fd = open(listener->address, O_RDONLY);

    read(listener_fd, &connectionRequest, sizeof(ConnectionRequest));

    close(listener_fd);

    sprintf(client_incoming, "/tmp/client_%d_incoming", connectionRequest.requester_pid);
    sprintf(client_outgoing, "/tmp/client_%d_outgoing", connectionRequest.requester_pid);

    connection->outgoing_fd = open(client_incoming, O_WRONLY);
    connection->incoming_fd = open(client_outgoing, O_RDONLY);

    return connection;

}

int comm_write(Connection * connection, char * dataToWrite, int size) {

    return write(connection->outgoing_fd, dataToWrite, size);

}

int comm_read(Connection * connection, char * dataToRead, int size) {

    return read(connection->incoming_fd, dataToRead, size);

}

int comm_disconnect(Connection * connection) {

    close(connection->incoming_fd);
    close(connection->outgoing_fd);

    return 0;

}