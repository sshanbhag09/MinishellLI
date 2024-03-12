#ifndef MSH_H
#define MSH_H

#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define DEBUG_ON                  0   //Set to 1 to enable debug messages
#define INPUT_BUF_SIZE            1024//(1 KB size)
#define ARG_DELIMITER             " \t\n\r\a"//Args separated by these characters
#define MAX_NO_OF_CMDS            100
#define MAX_CWD_LEN               1000
#define MAX_SINGLE_CMD_LEN        100

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
int exit_status;      //Exit status to demonstrate echo family commands
char PS1[MAX_CWD_LEN],//String to hold PS1 argument
     executable_path[MAX_CWD_LEN],//String to store executable path
	 no_ps1_prompt;   //Prompt PS1 if provoded, otherwise display current working directory

/* Structure to hold the jobs, works like stack */
typedef struct job_t
{
		char cmd[100];//Command name
		pid_t pid;    //Process ID
		int job_cnt;  //Job count
		struct job_t* link;//Self referential link
}t_job;

/* Function Prototypes */
void populateCommand();
void insertJob(t_job** top, char* job_name, pid_t job_pid);
t_job* getJob(t_job* top);
void printJobs(t_job* top);
void welcomeScreen();
void handleUserCommand(char* args[], char* read_command, int* no_of_args);
int numOfPipes(char* args[], int no_of_args);
void executeNPipes(int argc, char* argv[], int num_pipes);
void validate_and_update_index(int pipe_index[], int argc, char* argv[]);
int checkBuiltInCommands(char* read_args[], int no_of_args);
void executeCommand(int argc, char* argv[], t_job** top);
void putJobForeground(t_job** top);
void putJobBackground(t_job** top);
void signalHandler_child(int signal_num, siginfo_t* info, void* old_act);

#endif

