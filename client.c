#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "types.h"
#include "comm.h"
#include "constants.h"

void run_session();

char user_input[100];
char first_word[50];
char second_word[50];
char * sp = NULL;

int session_state;  // END_SESSION, CHAR_SELECTION, PLAY_GAME

Connection * connection;
Data * data_from_server;
Data * data_to_send;

char character[100];
int totalExp;

void main(int argc, char *argv[]) {

    printf("[client] connecting to server on address %s\n", CONNECTION_ADDRESS);

    connection = comm_connect(CONNECTION_ADDRESS);

    printf("[client] connection established\n");

    run_session();

    printf("[client] session done\n");

}

void run_session() {

    session_state = CHAR_SELECTION;

    while(1) {

        printf("[client] enter command: ");

        fgets(user_input, BUFFERSIZE, stdin);

        sp = strchr(user_input, ' ');

        if(sp == NULL) {

            sp = strchr(user_input, '\n');
            *sp = '\0';
            strcpy(first_word, user_input);

        } else {

            *sp++ = '\0';
            strcpy(first_word, user_input);
            strcpy(second_word, sp);

        }

        if(strcmp(first_word, "quit") == 0) {

            if(session_state == PLAY_GAME) {

                data_to_send = newData(EXIT_AND_LOGOUT);

                strcpy(data_to_send->character.name, character);

                sendData(connection, data_to_send);

            } else {

                sendData(connection, newData(EXIT));

            }

            data_from_server = receiveData(connection);

            comm_disconnect(connection);

            return ;

        } 

        else if(strcmp(first_word, "createchar") == 0) {

            if(session_state == PLAY_GAME) {

                printf("[client] cannot create a character. you are currently in a game\n");

            } else {

                data_to_send = newData(CREATE_CHARACTER);

                strcpy(data_to_send->character.name, second_word);

                sendData(connection, data_to_send);

                data_from_server = receiveData(connection);

                if(data_from_server->opcode == CHAR_ALREADY_EXISTS) {

                    printf("[client] character already exists. try again\n");

                } else {

                    printf("[client] character created succesfully.\n");

                }

            }

        } 

        else if(strcmp(first_word, "selectchar") == 0) {

            if(session_state == PLAY_GAME) {

                printf("[client] you already have a character selected\n");

            } else {

                data_to_send = newData(SELECT_CHARACTER);

                strcpy(data_to_send->character.name, second_word);

                sendData(connection, data_to_send);

                data_from_server = receiveData(connection);

                if(data_from_server->opcode == CHAR_DOESNT_EXIST) {

                    printf("[client] character doesnt exist. try again.\n");

                } else if(data_from_server->opcode == CHAR_BUSY) {

                    printf("[client] character is being played with by another client\n");

                } else {

                    printf("[client] successfully loaded character. current xp = %d\n", data_from_server->character.totalExp);

                    strcpy(character, data_from_server->character.name);

                    totalExp = data_from_server->character.totalExp;

                    session_state = PLAY_GAME;

                }

            }

        } 

        else if(strcmp(first_word, "p") == 0) {

            if(session_state == CHAR_SELECTION) {

                printf("[client] must first load a character\n");

            } else {

                totalExp++;

                data_to_send = newData(EXP_UP);

                data_to_send->character.totalExp = totalExp;
                
                strcpy(data_to_send->character.name, character);

                sendData(connection, data_to_send);

                data_from_server = receiveData(connection);

                printf("[client] your total exp is now: %d\n", data_from_server->character.totalExp);

            }
        
        }

    }

}