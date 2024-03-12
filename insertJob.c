#include "msh.h"

void insertJob(t_job** top, char* job_name, pid_t job_pid)
{
		//Create a new node
		t_job* new_job = malloc(sizeof(t_job));
		//Check if node is created or not
		if (new_job == NULL)
		{
				fprintf(stderr, "Error: Unable to allocate dynamic memory!\n\n");
				return;
		}
		//Update the data
		new_job -> pid = job_pid;

		/* Getting the current job count */
		t_job* old_job;//Pointer to hold old job address
		old_job = getJob(*top);
		if (old_job)
		{
			new_job -> job_cnt = old_job -> job_cnt + 1;//Current count is 1 greater than previous
		}
		else
		{
			new_job -> job_cnt = 1;//First job
		}

		strcpy(new_job -> cmd, job_name);//Store command string
		//Check whether the list is empty
		if (*top == NULL)//No elements in the linked list
		{
				//Update the link to NULL address
				new_job -> link = NULL;
				//printf("command: %s\n", new_job->cmd);
				//printf("count %d\n", new_job->job_cnt);
				//Update top
				*top = new_job;
				return;
		}
		//Establish a link between first and next node + update top pointer
		new_job -> link = *top;
		*top = new_job;
}

