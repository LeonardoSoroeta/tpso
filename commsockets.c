#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include "types.h"
#include "comm.h"

struct Connection {

    int socket_descriptor;

};

struct Listener {

    int listener_fd;

};

Connection * comm_connect(char * address) {

    int socket_fd, len;
    struct sockaddr_un remote;
    Connection * connection;

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, address);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);

    if ((socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {

        return NULL;

    }

    if ((connect(socket_fd, (struct sockaddr *)&remote, len)) == -1) {

        close(socket_fd);

        return NULL;
        
    }

    connection = malloc(sizeof(Connection));

    connection->socket_descriptor = socket_fd;

    return connection;

}

Listener * comm_listen(char * address) {

    int socket_fd, len;
    struct sockaddr_un local;
    Listener * listener;

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, address);
    len = strlen(local.sun_path) + sizeof(local.sun_family);

    if ((socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {

        return NULL;

    }

    unlink(local.sun_path);

    if (bind(socket_fd, (struct sockaddr *)&local, len) == -1) {

        close(socket_fd);

        return NULL;
    }

    if (listen(socket_fd, MAXQ_SIZE) == -1) {

        close(socket_fd);

        return NULL;
    }

    listener = malloc(sizeof(Listener));

    listener->listener_fd = socket_fd;

    return listener;

}

Connection * comm_accept(Listener * listener) {

    int socket_fd, len;
    struct sockaddr_un remote;
    Connection * connection;

    len = sizeof(struct sockaddr_un);

    if ((socket_fd = accept(listener->listener_fd,  (struct sockaddr *)&remote, (socklen_t*)&len)) == -1) {

        return NULL;

    }

    connection = malloc(sizeof(Connection));

    connection->socket_descriptor = socket_fd;

    return connection;

}

int comm_write(Connection * connection, char * dataToWrite, int size) {

    return write(connection->socket_descriptor, dataToWrite, size);

}

int comm_read(Connection * connection, char * dataToRead, int size) {

    return read(connection->socket_descriptor, dataToRead, size);

}

int comm_disconnect(Connection * connection) {

    return close(connection->socket_descriptor);

}
