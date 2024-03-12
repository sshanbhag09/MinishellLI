#include "msh.h"

void putJobForeground(t_job** top)
{
		//Check if stack is empty
		if (*top == NULL)
		{
				printf("msh: fg: current: no such job\n");
				return;//Explicit return
		}
		else
		{
				printf("%s\n", (*top) -> cmd);//Print given command
				kill((*top) -> pid, SIGCONT);//Pass the SIGCONT in kill system call to restart the process with most recent job pid
				waitpid((*top) -> pid, NULL, 0);//Wait for job to finish
				//Free the node
				t_job* to_be_freed = *top;
				*top = (*top) -> link;
				free(to_be_freed);
		}
		//Implicit return
}
