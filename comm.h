#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

typedef struct Connection Connection;

typedef struct Listener Listener;

Connection * comm_connect(char * address);

Listener * comm_listen(char * address);

Connection * comm_accept(Listener * listener);

int comm_write(Connection * connection, char * dataToWrite, int size);

int comm_read(Connection * connection, char * dataToRead, int size);

int comm_disconnect(Connection * connection);

#endif
