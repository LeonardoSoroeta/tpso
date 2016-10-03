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
#include "databasecomm.h"

struct DBConnection {

    int incoming_fd;
    int outgoing_fd;

};

struct DBListener {

    char address[32];

};

typedef struct {

    int requester_pid;

} DBConnectionRequest;

DBConnection * db_comm_connect(char * address) {

    int listener_fd;
    char client_incoming[32];
    char client_outgoing[32];
    DBConnectionRequest db_connectionRequest;
    DBConnection * db_connection;

    db_connectionRequest.requester_pid = getpid();

    sprintf(client_incoming, "/tmp/client_%d_incoming", db_connectionRequest.requester_pid);
    sprintf(client_outgoing, "/tmp/client_%d_outgoing", db_connectionRequest.requester_pid);

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

    write(listener_fd, &db_connectionRequest, sizeof(DBConnectionRequest));

    close(listener_fd);

    db_connection = malloc(sizeof(DBConnection));

    db_connection->incoming_fd = open(client_incoming, O_RDONLY);
    db_connection->outgoing_fd = open(client_outgoing, O_WRONLY);

    return db_connection;

}

DBListener * db_comm_listen(char * address) {

    DBListener * db_listener;

    db_listener = malloc(sizeof(DBListener));

    unlink(address);

    if (mknod(address, S_IFIFO | 0666, 0) == -1) {
      perror("Couldn´t create server private named pipe. Error");
      return NULL;
    }

    strcpy(db_listener->address, address);

    return db_listener;
}

DBConnection * db_comm_accept(DBListener * db_listener) {

    int listener_fd;
    char client_incoming[32];
    char client_outgoing[32];
    DBConnectionRequest db_connectionRequest;
    DBConnection * db_connection;

    db_connection = malloc(sizeof(DBConnection));

    listener_fd = open(db_listener->address, O_RDONLY);

    read(listener_fd, &db_connectionRequest, sizeof(DBConnectionRequest));

    close(listener_fd);

    sprintf(client_incoming, "/tmp/client_%d_incoming", db_connectionRequest.requester_pid);
    sprintf(client_outgoing, "/tmp/client_%d_outgoing", db_connectionRequest.requester_pid);

    db_connection->outgoing_fd = open(client_incoming, O_WRONLY);
    db_connection->incoming_fd = open(client_outgoing, O_RDONLY);

    return db_connection;

}

int db_comm_write(DBConnection * db_connection, char * dataToWrite, int size) {

    return write(db_connection->outgoing_fd, dataToWrite, size);

}

int db_comm_read(DBConnection * db_connection, char * dataToRead, int size) {

    return read(db_connection->incoming_fd, dataToRead, size);

}

int db_comm_disconnect(DBConnection * db_connection) {

    close(db_connection->incoming_fd);
    close(db_connection->outgoing_fd);

    return 0;

}