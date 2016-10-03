#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"


char * getaddress() {

	char * line;

	char buffer[BUFF_SIZE];

	size_t len=0;

	FILE * file;

	file = fopen("commconfig.txt", "r");

	if (file) {

		rewind(file);

		while ((getline(&line, &len, file)) != -1) {

	        if (sscanf(line, "Address : %s", buffer)>0) {

	        	char * string = malloc(strlen(buffer) +1);
	        	strcpy(string, buffer);
	        	return string;
	        }
		}
	}
	return NULL;
}

unsigned int getkey(char * alias) {

		int hex;

		FILE * file;

		char buffer[BUFF_SIZE];

		char * line;

		char * aux;

		size_t len=0;

		file = fopen("commconfig.txt", "r");

		if (file) {

			rewind(file);

			aux = (char *) malloc(KEY_LENGTH + strlen(alias) + 1);

			strcpy(aux, "Key");

			strcat(aux, alias);

			while ((getline(&line, &len, file)) != -1) {

		        sscanf(line, "%s : %x", buffer, &hex);
		        
		        if (strcmp(buffer, aux) == 0) {
		        	return hex;
		        }
	    
		    }
		    printf("That alias does not exist as a key-value in commconfig.txt\n");
	    }
	    printf("FILE couldn´t be opened\n");
		exit(1);
}


