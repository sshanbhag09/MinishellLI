#include "msh.h"
#include <errno.h>

void executeCommand(int argc, char* argv[], t_job** top)
{
		//String to hold the command name
		char job_name[MAX_SINGLE_CMD_LEN] = "";
		int i = 1;
		while (argv[i] != NULL)
		{
				strcat(job_name, argv[i++]);
				strcat(job_name, " ");
		}
		//printf("Job name: %s\n", job_name); //For Debugging
		//To change the prompt using PS1, compare first 4 characters of the arg
		if (!strncmp(argv[1], "PS1=", 4) && strncmp(argv[1], "PS1= ", 5))
		{
				strcpy(PS1, argv[1] + 4);//Copy the user given string into PS1
				no_ps1_prompt = 0;//Change the prompt style to PS1 type
				return;
		}
		else if (!strcmp(argv[1], "fg"))//Check for fg command
		{
				putJobForeground(top);//Put the most recent job in foreground
				return;
		}
		else if (!strcmp(argv[1], "bg"))//Check for bg command
		{
				putJobBackground(top);//Put the most recent job in background
				return;
		}
		else if (!strcmp(argv[1], "jobs"))//Check for jobs command
		{
				printJobs(*top);//Print the job status
				return;
		}
		pid_t pid1 = fork();//Create another process
		if (pid1)
		{
				//Parent
				int child_status;
				int ret_flag = 1;//Flag to detect end of waiting 
				while (ret_flag)
				{
						if (waitpid(pid1, &child_status, WUNTRACED) == -1)
						{
								//Upon receiving -1, EINTR error is obtained
								if (errno != EINTR)
								{
										ret_flag = 0;//Reset the flag
								}
						}
						else
						{
								ret_flag = 0;//Reset the flag
						}
				}
				//Get child exit status
				if (WIFEXITED(child_status))
				{
						exit_status = WEXITSTATUS(child_status);
				}
				/*//Debug message if child is aborted
				   else
				   {
				   printf("Process aborted\n");
				   }
				 */
				if (WIFSTOPPED(child_status))//Check if TSTP signal is received
				{
						insertJob(top, job_name, pid1);//Insert new job
						t_job* temp = getJob(*top);//Get the most recent job
						//printf("Child process %d stopped\n", pid1);
						printf("^Z\n[%d]+  Stopped\t\t%s\n", temp -> job_cnt, temp -> cmd);
						fflush(stdout);
				}
		}
		else
		{
				//Child
#if DEBUG_ON
				printf("\nThis is child process with id %d\n", getpid());
				fflush(stdout);
#endif
				//argv[argc] = NULL;
				//Execute command
				if (execvp(argv[1], argv + 1) == -1)
				{
						//Prompt error message on unsuccessful cmd execution
						perror("msh error");
						exit(EXIT_FAILURE);
				}	
		}
}

