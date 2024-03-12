#include "msh.h"

/* Function to validate and update the pipe index array */
void validate_and_update_index(int pipe_index[], int argc, char* argv[])
{
		//Check if 1st or last argument is pipe
		if (!strcmp(argv[1], "|"))
		{
				fprintf(stderr, "Error: First argument cannot be pipe argument!\n\n");
				return;
		}
		else if (!strcmp(argv[argc - 1], "|"))
		{
				fprintf(stderr, "Error: Last argument cannot be pipe argument!\n\n");
				return;
		}
		int pipe_num = 0,//To hold index num of pipe
			index = 2;//Start checking for pipe from index 2
		while (index != argc)
		{
				//Check if command followed by pipe is found
				//Note the string comparison operation
				if (!strcmp(argv[index], "|") && strcmp(argv[index - 1], "|"))
				{
						pipe_index[pipe_num++] = index;
#if DEBUG_ON
						printf("Pipe-%d found at argv index = %d\n", pipe_num, index);
						fflush(stdout);
#endif
				}
				index++;
		}
}

void executeNPipes(int argc, char* argv[], int num_pipes)
{
		//Array to hold index of n-pipes
		int pipe_index[num_pipes];
		validate_and_update_index(pipe_index, argc, argv);
		if (argc < 4 || num_pipes < 1)//Error if invalid arguments are given
		{
				fprintf(stderr, "\nError: Invalid command line arguments!\nGeneral usage: ./pipe <command1> '|' <command2> '|' <command3> '|' ...\n\n");
				return;
		}
#if DEBUG_ON
		printf("No. of pipes found = %d\n", num_pipes);
		//Very important to flush stdout as we will be duplicating it later
		fflush(stdout);
#endif
		int cpy_of_stdin = dup(0);
		// Create pipe fds equivalent to no. of pipes
		int pipe_fds[num_pipes][2];
		for (int i = 0; i < num_pipes; i++)
		{
				pipe(pipe_fds[i]);
		}
		//Run a loop to create child processes equal to no. of commands
		//Note that we have (n) pipes and (n + 1) commands
		for (int i = 0; i < num_pipes + 1; i++)
		{
				if (i)//For non-zero positive i, close previous pipe's write end 
				{
						close(pipe_fds[i - 1][1]);
						//Duplicate stdin with previous pipe's read end except when it is last command since no pipe exists for last command
						if (i != num_pipes)
						{
								close(0);
								dup(pipe_fds[i - 1][0]);
						}
				}
				if (fork())//Create Child process by using fork() system call
				{
						//Parent
						//wait() in parent will perform each command in a sequence, multiprocessing will not take place
						//wait(NULL);
				}
				else
				{
						//Child
						//Check if it is 1st child
						if (i == 0)
						{
								//Put NULL at 1st pipe index
								argv[pipe_index[0]] = NULL;
								//Close read end of pipe1 & duplicate stdout with its write end
								close(pipe_fds[0][0]);
#if DEBUG_ON
								printf("\nFirst command!\nFirst pipe index = %d\n", pipe_index[0]);
								fflush(stdout);
#endif
								//Duplicate stdout as write end of 1st pipe
								close(1);
								dup(pipe_fds[0][1]);
								if (execvp(argv[1], argv + 1) == -1)
								{
										//Detect the execvp error
										perror("msh error");
										//return;
								}
						}
						//Otherwise check if it is last child
						else if (i == num_pipes)
						{
								argv[argc] = NULL;//This line can be removed
								//Close write end of second last pipe
								close(pipe_fds[num_pipes - 1][1]);
								//Duplicate stdin as read end of last pipe
								close(0);
								dup(pipe_fds[num_pipes - 1][0]);
#if DEBUG_ON
								printf("Last command begins with %s!\nLast pipe index = %d\n", argv[(pipe_index[num_pipes - 1]) + 1], pipe_index[num_pipes - 1]);
								fflush(stdout);
#endif
								if (execvp(argv[(pipe_index[num_pipes - 1]) + 1], (argv + pipe_index[num_pipes - 1] + 1)) == -1)
								{
										perror("msh error");
										//return;
								}

						}
						//Middle child
						else
						{
								//Assign NULL to the argument where previous pipe is found and pass the next argument's address to execute the command
								argv[pipe_index[i]] = NULL;
#if DEBUG_ON
								printf("Middle command!\nPrevious pipe index = %d\n", pipe_index[i]);
								fflush(stdout);
#endif
								//close read end of current pipe
								close(pipe_fds[i][0]);
								//Duplicate stdout with current pipe's write end
								close(1);
								dup(pipe_fds[i][1]);
								if (execvp(argv[pipe_index[i - 1] + 1], (argv + pipe_index[i - 1] + 1)) == -1)
								{
										fprintf(stderr, "msh: %s", argv[i - 1] + 1);
										perror("msh error");
										//return;
								}
						}
				}
		}
		dup2(cpy_of_stdin, 0);
		//Clean up the resources of all the children after finishing all the processes using wait(). This will do multiprocessing. The output will be correct since pipe is naturally synchronized IPC method.
		for (int i = 0; i < num_pipes + 1; i++)
		{
				wait(NULL);
				//perror("wait");
		}
}

