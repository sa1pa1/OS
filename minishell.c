/*********************************************************************
   Program  : miniShell                   Version    : 1.3
 --------------------------------------------------------------------
   skeleton code for linix/unix/minix command line interpreter
 --------------------------------------------------------------------
   File			: minishell.c
   Compiler/System	: gcc/linux

********************************************************************/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define NV 20			/* max number of command tokens */
#define NL 100			/* input buffer size */
char            line[NL];	/* command input buffer */


/*
	shell prompt
 */

void prompt(void)
{
  fflush(stdout);
}

void background_handler(int signal)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        fprintf(stdout, "Background process %d finished\n", pid);
    }
}


int main(int argk, char *argv[], char *envp[])
/* argk - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */

{
  int             frkRtnVal;	/* value returned by fork sys call */
  int             wpid;		/* value returned by wait */
  char           *v[NV];	/* array of pointers to command line tokens */
  char           *sep = " \t\n";/* command line token separators    */
  int             i;		/* parse index */
  int             bp;       /*background process*/

  signal(SIGCHLD, background_handler);

  /* prompt for and process one command line at a time  */

  while (1) {			/* do Forever */
    prompt();
    fgets(line, NL, stdin);
    fflush(stdin);
    if (fgets(line, NL, stdin) == NULL) {
                if (feof(stdin)) {  /* non-zero on EOF  */
                    exit(0);
                } else {
                    perror("fgets");
                    continue;
                }
            }
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
      continue;			/* to prompt */

    v[0] = strtok(line, sep);
    for (i = 1; i < NV; i++) {
      v[i] = strtok(NULL, sep);
      if (v[i] == NULL)
	break;
    }
    /* assert i is number of tokens + 1 */

    if (strcmp(v[0], "cd") == 0) {
            if (v[1] == NULL || chdir(v[1]) != 0) {
                perror("cd");
            }
            continue;
        }

        bp = (i > 1 && strcmp(v[i-1], "&") == 0);
        if (bp) {
            v[i-1] = NULL;
        }

    /* fork a child process to exec the command in v[0] */

    switch (frkRtnVal = fork()) {
    case -1:			/* fork returns error to parent process */
      {
    perror("fork");
	break;
      }
    case 0:			/* code executed only by child process */
      {
	if (execvp(v[0], v) == -1) {
                perror("execvp fail");
                exit(EXIT_FAILURE);
            }
	
      }
    default:			/* code executed only by parent process */
      {
    if (bp) {
                printf("Background process %d started\n", frkRtnVal);
            } else {
                wpid = wait(0);
                if (wpid == -1) {
                    perror("wait");
                }
                printf("%s done\n", v[0]);
            }
	break;
      }
    }				/* switch */
  }				/* while */
}				/* main */
