#ifndef _TYPES_H
#define _TYPES_H

#include "constants.h"

typedef int Opcode;

typedef struct {
		char name[SIZE];
		int totalExp;
} Character;

typedef struct __attribute__((__packed__)) {
	int sender_pid;
	Opcode opcode;
	Character character;
} Data;

typedef struct __attribute__((__packed__)) {
	char * payload;
} Datagram;

#endif
