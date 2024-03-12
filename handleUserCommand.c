#include "msh.h"

void handleUserCommand(char* args[], char* read_command, int* no_of_args)
{
		int i = 1;//Iterator 'i'
		char* token = strtok(read_command, ARG_DELIMITER);
		while (token != NULL)
		{
				args[i++] = token;
			    ++(*no_of_args);
				token = strtok(NULL, ARG_DELIMITER);
		}
}

