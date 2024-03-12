#include "msh.h"

void delJob(t_job** top)
{
		//Check whether the list is empty
		if (*top == NULL)//No elements in the linked list
		{
				return;
		}
		//Create a pointer to store the address of first node
		t_job* temp = *top;
		//Update top pointer
		*top = temp -> link;
		//Free the removed node
		free(temp);
       //Let's make freed pointer point to NULL address
		temp = NULL;
}

