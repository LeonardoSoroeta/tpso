#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"


char * getaddress(char * alias) {

	FILE * file;

	char buffer1[BUFF_SIZE];

	char buffer2[BUFF_SIZE];

	char * line;

	char * aux;

	size_t len=0;

	file = fopen("commconfig.txt", "r");

	if (file) {

		rewind(file);

		aux = (char *) malloc(strlen("Address") + strlen(alias) + 1);

		strcpy(aux, "Address");

		strcat(aux, alias);

		//printf("%s\n", aux);

		while ((getline(&line, &len, file)) != -1) {

			//printf("%s\n", line);

	        sscanf(line, "%s : %s", buffer1, buffer2);
	        
	        if (strcmp(buffer1, aux) == 0) {
				char * string = malloc(strlen(buffer2) +1);
	        		strcpy(string, buffer2);
	        		free(aux);
	        		return string;
	        	}	        
    
	    }

	    printf("That alias does not exist as a address-value in commconfig.txt\n");

    }

    else {
    	printf("commconfig.txt couldn´t be opened\n");
		exit(1);

    }

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


