#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include "sqlite3.h"
#include "comm.h"
#include "types.h"
#include "constants.h"


void initialize_table(void);
int callback(void *, int, char **, char **);
void create_char();
void select_char();
void logout_char();
void char_exp_up();

sqlite3 *db;
char *err_msg = 0;
char query[200];
int rc;
int semaphore_id;
Listener * listener;
Connection * connection;
Data * data;

int main(void) {

    printf("[database] initializing\n");
    
    initialize_table();

    listener = comm_listen("/tmp/database_channel");

    semaphore_id = binary_semaphore_allocation (666, IPC_RMID);

    binary_semaphore_initialize (semaphore_id);

    printf("[database] awaiting connections\n");

    while(1) {

        connection = comm_accept(listener);

        data = receiveData(connection);

        process_data();

        sendData(connection, data);

        comm_disconnect(connection);

    }

    binary_semaphore_deallocate(semaphore_id);
    
    sqlite3_close(db);
    
    return 0;

}

void initialize_table() {

    unlink("/tmp/database.db");

    int rc = sqlite3_open("/tmp/database.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", 
                sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }

    char *sql = "CREATE TABLE Chars(Id TEXT PRIMARY KEY, Exp INTEGER, Inuse INTEGER);";
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        
    }

}

void process_data() {

    if(data->opcode == SELECT_CHARACTER) {

        select_char();

    } 

    else if(data->opcode == CREATE_CHARACTER) {

        create_char();

    }

    else if(data->opcode == EXP_UP) {

        char_exp_up();

    }

    else if(data->opcode == EXIT_AND_LOGOUT) {

        logout_char();

    }

}

void create_char() {

    printf("creating char %s\n", data->character.name);

    sprintf(query, "INSERT INTO Chars VALUES('%s', 666, 0)", data->character.name);   

    rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {

        data->opcode = CHAR_ALREADY_EXISTS;
        
        return ;

    } 

    data->opcode = NO_ERROR;

}

void select_char() {

    printf("selecting char %s\n", data->character.name);

    sprintf(query, "SELECT * FROM Chars WHERE Id='%s'", data->character.name);

    rc = sqlite3_exec(db, query, callback, 0, &err_msg);

    if(data->opcode == SELECT_CHARACTER) {

        data->opcode = CHAR_DOESNT_EXIST;

    }

}

int callback(void *NotUsed, int argc, char **argv, char **azColName) {

    if(atoi(argv[2]) == 1) {

        data->opcode = CHAR_BUSY;

    } else {

        sprintf(query, "UPDATE Chars SET Inuse=1 WHERE Id='%s'", data->character.name);

        sqlite3_exec(db, query, 0, 0, &err_msg);

        data->character.totalExp = atoi(argv[1]);

        data->opcode = NO_ERROR;
    }

    return 0;
}

void logout_char() {

    sprintf(query, "UPDATE Chars SET Inuse=0 WHERE Id='%s'", data->character.name);

    sqlite3_exec(db, query, 0, 0, &err_msg);

    data->opcode = NO_ERROR;

}

void char_exp_up() {

    sprintf(query, "UPDATE Chars SET Exp=%d WHERE Id='%s'", data->character.totalExp, data->character.name);

    sqlite3_exec(db, query, 0, 0, &err_msg);

    data->opcode = NO_ERROR;

}