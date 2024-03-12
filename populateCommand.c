#include "msh.h"

void populateCommand(char* read_command)
{
		//Let's allocate buffer size memory dynamically (1 KB)
		if (read_command == NULL)
		{
				fprintf(stderr, "\nError: Unable to allocate dynamic memory!\n\n");
				exit(1);
		}
		int i = 0;//Iterator 'i'
		while (1)
		{
				//Read each character from stdin
				int c =  getchar();
				if (c == EOF)
				{
						//printf("getchar check\n");
						read_command[i] = '\0';
						if (feof(stdin))
						{
								printf("\n");
								exit(EXIT_SUCCESS);
						}
						return;
				}
				else if (c == '\n')
				{
						read_command[i] = '\0';
						return;
				}
				else
				{
						read_command[i] = c;
				}
				i++;
				//Extend buffer size using realloc if needed
				if (i >= INPUT_BUF_SIZE)
				{
						char* temp = realloc(read_command, 2 * INPUT_BUF_SIZE);
						if (temp == NULL)
						{
								fprintf(stderr, "\nError: Unable to allocate dynamic memory!\n\n");
								return;
						}
						read_command = temp;
				}
		}
}

