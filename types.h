#ifndef _TYPES_H
#define _TYPES_H

#include "constants.h"

typedef int Opcode;

typedef struct {
		char name[SIZE];
		int totalExp;
} Character;

typedef struct {
	int sender_pid;
	Opcode opcode;
	Character character;
} Data;

typedef struct {
	char * payload;
} Datagram;

#endif
