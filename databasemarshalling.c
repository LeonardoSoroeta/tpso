#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "types.h"
#include "databasecomm.h"


Datagram * db_marshall(Data * data) {

	Datagram * datagram;

	datagram = malloc(sizeof(Datagram));
	datagram->payload = malloc(sizeof(Data));

	memcpy(datagram->payload, data, sizeof(Data));

	return datagram;

}

Data * db_unmarshall(Datagram * datagram) {

	Data * data;

	data = malloc(sizeof(Data));

	memcpy(data, datagram->payload, sizeof(Data));

	return data;

}

int db_sendData(DBConnection * db_connection, Data * data) {

	Datagram * datagram;

	data->sender_pid = getpid();

	datagram = db_marshall(data);

	return db_comm_write(db_connection, datagram->payload, sizeof(Data));
}

Data * db_receiveData(DBConnection * db_connection) {

	Datagram * datagram;
	Data * data;

	datagram = malloc(sizeof(Datagram));
	datagram->payload = malloc(sizeof(Data));

	if (db_comm_read(db_connection, datagram->payload, sizeof(Data))<0)
		return NULL;

	data = db_unmarshall(datagram);

	return data;

}

Data * db_newData(Opcode opcode) {

	Data * data;
	
	data = malloc(sizeof(Data));

	data->opcode = opcode;

	return data;

}
