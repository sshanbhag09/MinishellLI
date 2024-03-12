#include "msh.h"

void putJobBackground(t_job** top)
{
		//Check if stack is empty
		if (*top == NULL)
		{
				printf("msh: bg: current: no such job\n");
				return;//Explicit return
		}
		printf("[%d]+ %s&\n", (*top) -> job_cnt, (*top) -> cmd);
		kill((*top) -> pid, SIGCONT);
		//No waiting since this job runs in background. So use WNOHANG
		waitpid((*top) -> pid, NULL, WNOHANG);
		t_job* to_be_freed = *top;
		*top = (*top) -> link;
		free(to_be_freed);
		//Implicit return
}

