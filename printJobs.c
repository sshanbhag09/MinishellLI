#include "msh.h"

void printJobs(t_job* top)
{
		//Check if stack is empty
		if (top == NULL)
		{
				return;//Explicit return
		}
		else
		{
				printJobs(top -> link);//Recursively print all jobs status
				printf("[%d]", top -> job_cnt);
				if (top -> link == NULL)
						printf("+  ");
				else if (top -> link && top -> link -> link == NULL)
						printf("-  ");
				else
						printf("   ");
				printf("Stopped\t\t%s\n", top -> cmd);
		}
}
