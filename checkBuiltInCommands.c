#include "msh.h"
#include <errno.h>

//System call demonstration
/* Function to check and execute built-in commands */
int checkBuiltInCommands(char* read_args[], int no_of_args)
{
		int i = 0;//Iterator
		static char prev_path[MAX_SINGLE_CMD_LEN] = {0};//String to update previous path, especially required to demonstrate "cd -" command
		char str[MAX_SINGLE_CMD_LEN] = {0};//Store actual cmd to be executed
		char temp_str[MAX_SINGLE_CMD_LEN] = {0};//Temporary string to hold previous path
#if DEBUG_ON
		while (i < no_of_args - 1)//Output the obtained command arguments
		{
				printf("Read args[%d] = %s\n", i, read_args[i]);	
				i++;
		}
#endif
		//Check if cd command is provided
		if (!strcmp(read_args[0], "cd"))
		{
				strcpy(temp_str, prev_path);//Store prev directory path temporarily
				//"cd" with no arguments takes to home directory
				if (read_args[1] == NULL)//If only "cd" is given, go to home directory
				{
						strcpy(str, getenv("HOME"));//Copy home directory path
						//getcwd(prev_path, MAX_CWD_LEN);
						//printf("Home path: %s\n", str);
				}
				// "cd -" takes to previous directory
				else if (!strcmp(read_args[1], "-"))
				{
						if (strlen(prev_path))
						{
								//Copy prev path to str
								strcpy(str, prev_path);
								//getcwd(prev_path, MAX_CWD_LEN);
						}
						else
						{
								//Old pwd not set
								fprintf(stderr, "msh error: cd: OLDPWD not set\n");
								return 1;//Return 1 to indicate built-in command
						}
				}
				else//Simply copy the command arguments in the cmd string 
				{
						strcpy(str, read_args[1]);
				}
				getcwd(prev_path, MAX_CWD_LEN);//Store current path in prev_path for further use
				//printf("Prev path = %s\n", prev_path);
				if (chdir(str) != 0)//execute cmd using chdir function
				{
						//Report specific error as per obtained error number
						if (errno == EACCES)
						{
							perror("Eaccess fault");
						}
						else if(errno == ENOENT)
						{
							perror("Enoent fault");
						}
						strcpy(prev_path, temp_str);//Hold back the prev_path from temp_str
				}
				return 1;//Return 1 to indicate built-in command
		}
		//Check if mkdir command is given
		else if (!strcmp(read_args[0], "mkdir"))
		{
				//If next argument is absent, it is an error
				if (read_args[1] == NULL)
				{
						fprintf(stderr, "mkdir: missing operand\n");
				}
				else if (mkdir(read_args[1], 0777) != 0)//call mkdir function
				{
						perror("msh error");//Report error if any
				}
				return 1;//Return 1 to indicate built-in command
		}
		//Check if rmdir command is given
		else if (!strcmp(read_args[0], "rmdir"))
		{
				//If next argument is absent, it is an error
				if (read_args[1] == NULL)
				{
						fprintf(stderr, "rmdir: missing operand\n");
				}
				else if (rmdir(read_args[1]) != 0)//Call to rmdir function
				{
						perror("msh error");//Report error if any
				}
				return 1;//Return 1 to indicate built-in command
		}
		//Check if exit command is given
		else if (!strcmp(read_args[0], "exit"))
		{
				exit(EXIT_SUCCESS);//Simply exit from mini shell using exit function
		}
		//Check if echo command is given
		else if (!strcmp(read_args[0], "echo"))
		{
				//To print the shell pid
				if (!strcmp(read_args[1], "$$"))
				{
						printf("%d\n", getpid());
				}
				//To print the previous process's exit status
				else if (!strcmp(read_args[1], "$?"))
				{
						printf("%d\n", exit_status);
				}
				//Prints the executable path
				else if (!strcmp(read_args[1], "$SHELL"))
				{
						printf("%s\n", executable_path);
				}
				return 1;//Return 1 to indicate built-in command
		}
		//Return 0 if it is none of the above commands
		return 0;
}

