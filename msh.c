/*
 *
 *           Name           : Ssuahnat Shanbhag
 *           Date           : March 12, 2024
 *           Descriptions   : minishell program capable of executing commands and job control
 *           Objective      : Implement own Linux Shell
*/

#include "msh.h"

int main()
{
		char cwd[MAX_CWD_LEN] = {0};//A string to read Current Working Directory
		char* args[MAX_NO_OF_CMDS] = {0};//Array of command arguments
		args[0] = "./msh.exe";//1st argument is executable process name
		int no_of_args = 1,//No. of arguments (Initially 1, considering above)
			num_pipes = 0;//No. of pipes ('|') provided by user (Initially 0)
		t_job* top = NULL;//Pointer to hold address of the most recent job
		no_ps1_prompt = 1;//Initialize gloabal PS1 tracker as 1
		//A string to store the entire input provided by user
		char* read_command = malloc(INPUT_BUF_SIZE * sizeof(char));
		getcwd(cwd, sizeof(cwd));//Project Directory path
        sprintf(executable_path, "%s/msh.exe", cwd);//Stores the current executable path
		/* Handling the signals */
		struct sigaction act, oldact;
		//Fill all the structure values with 0
		memset(&act, 0, sizeof(act));
		act.sa_flags = SA_NOCLDSTOP | SA_NOCLDWAIT | SA_SIGINFO;//SA_SIGINFO is must for three argument signal handler
		act.sa_sigaction = signalHandler_child;//Three argument function
		sigaction(SIGINT, &act, &oldact);
		sigaction(SIGTSTP, &act, &oldact);
		//Quit signal: Ctrl + \ (Comment out for debug; To move out of msh)

		do
		{
				getcwd(cwd, sizeof(cwd));//Read the current working directory
				//Formating the prompt message
				if (no_ps1_prompt)
				{   
						sprintf(PS1, "msh-0xxx\033[0m:\033[1;34m%s\033[0m$ ", cwd);
				}
				printf("%s", PS1);//Print the current working directory
				fflush(stdout);
				//Read the entire string provided by user
				populateCommand(read_command);
#if DEBUG_ON
				printf("Command: %s\n", read_command);
				fflush(stdout);
#endif
				//Segregate each command argument
				handleUserCommand(args, read_command, &no_of_args);
#if DEBUG_ON
				printf("No. of args = %d\n", no_of_args);
				for (int i = 0; i < no_of_args; i++)
				{
						printf("Arg %d: %s\n", i, args[i]);
				}
				fflush(stdout);
#endif
				//Check if any extra argument is provided
				if (no_of_args > 1)
				{
						//Run nPipes() if pipe cmds ('|') are given
						if (num_pipes = numOfPipes(args, no_of_args))
						{
								//Execute (n-pipe)--(n + 1) cmds
								executeNPipes(no_of_args, args, num_pipes);
						}
						//Check for built-in cmd else execute the cmd
						else if (!checkBuiltInCommands(args + 1, no_of_args))
						{
								executeCommand(no_of_args, args, &top);
						}
				}
				//Reset read_command string as 0
				memset(read_command, 0, INPUT_BUF_SIZE * sizeof(char));
				//Reset argument string except the first argument
				memset(args + 1, 0, (MAX_NO_OF_CMDS - 1) * sizeof(char*));
				no_of_args = 1;//No. of arguments changes back to 1
		} while (1);
		free(read_command);//Free up memory
		return 0;
}

