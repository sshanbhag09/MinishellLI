#include "msh.h"

void signalHandler_child(int signal_num, siginfo_t* info, void* old_act)
{
		//Exit status in general is 128 + signal_number
		exit_status = 128 + signal_num;
		if (signal_num == SIGINT)
		{
				no_ps1_prompt = 1;//Prompt back to current working directory
				printf("\n");
		}
		//Simply put a carriage return charater for remaining signals
		else if (signal_num == SIGTSTP)
		{
				printf("\r");
		}
}

