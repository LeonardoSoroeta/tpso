#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "types.h"
#include "comm.h"


Datagram * marshall(Data * data) {

	Datagram * datagram;

	datagram = malloc(sizeof(Datagram));
	datagram->payload = malloc(sizeof(Data));

	memcpy(datagram->payload, data, sizeof(Data));

	return datagram;

}

Data * unmarshall(Datagram * datagram) {

	Data * data;

	data = malloc(sizeof(Data));

	memcpy(data, datagram->payload, sizeof(Data));

	return data;

}

int sendData(Connection * connection, Data * data) {

	Datagram * datagram;

	data->sender_pid = getpid();

	datagram = marshall(data);

	return comm_write(connection, datagram->payload, sizeof(Data));
}

Data * receiveData(Connection * connection) {

	Datagram * datagram;
	Data * data;

	datagram = malloc(sizeof(Datagram));
	datagram->payload = malloc(sizeof(Data));

	if (comm_read(connection, datagram->payload, sizeof(Data))<0)
		return NULL;

	data = unmarshall(datagram);

	return data;

}

Data * newData(Opcode opcode) {

	Data * data;
	
	data = malloc(sizeof(Data));

	data->opcode = opcode;

	return data;

}
