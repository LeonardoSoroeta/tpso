#ifndef _DBCOMMUNICATION_H
#define _DBCOMMUNICATION_H

typedef struct DBConnection DBConnection;

typedef struct DBListener DBListener;

DBConnection * db_comm_connect(char * address);

DBListener * db_comm_listen(char * address);

DBConnection * db_comm_accept(DBListener * db_listener);

int db_comm_write(DBConnection * db_connection, char * dataToWrite, int size);

int db_comm_read(DBConnection * db_connection, char * dataToRead, int size);

int db_comm_disconnect(DBConnection * db_connection);

#endif
