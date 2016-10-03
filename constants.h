
#ifndef _CONSTANTS_H
#define _CONSTANTS_H

typedef enum { false, true } bool;

/*Game States*/
#define CHAR_SELECTION 1
#define PLAY_GAME 2

/*Opcodes */
#define SELECT_CHARACTER 2
#define CREATE_CHARACTER 3
#define DELETE_CHARACTER 4
#define EXP_UP 14

#define CHAR_ALREADY_EXISTS 1
#define CHAR_DOESNT_EXIST 2
#define CHAR_SELECT_SUCCESFUL 3
#define CHAR_BUSY 4 
#define NO_ERROR 500

#define EXIT 200
#define EXIT_AND_LOGOUT 300

/*length or sizes*/
#define CANT_ARGS 5
#define COMM_SIZE 11
#define STATES 6
#define BLOCK 10
#define LEVEL_MAX 10
#define MSG_LEN 50
#define BUFFERSIZE 200
#define MAXQ_SIZE 5
#define MAX_THREADS 10
#define MAX_CHARACTERS 5
#define SIZE 20
#define KEY_LENGTH 3
#define BUFF_SIZE 20

#endif
