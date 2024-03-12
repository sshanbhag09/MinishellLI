#include "msh.h"

int numOfPipes(char* args[], int no_of_args)
{
		//Check if 1st argument 
		if (!strcmp(args[0], "|"))
		{
				fprintf(stderr, "msh: syntax error near unexpected token `|'\n");
				exit(1);
		}
		//Initial count of pipes = 0, iterating from index 1 to (argc - 1)
		int no_of_pipes = 0, i = 0;
		while (i < no_of_args)
		{
				//Compare each argument with pipe '|' & increment the count if found
				if (!strcmp(args[i++], "|"))
						no_of_pipes++;
		}
		return no_of_pipes;

}

