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
char line[NL];	/* command input buffer */


/*
	shell prompt
 */

prompt(void)
{
  fprintf(stdout, "\n msh> ");
  fflush(stdout);

}


main(int argk, char *argv[], char *envp[])
/* argk - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */

{
  int             frkRtnVal;	/* value returned by fork sys call */
  int             wpid;		/* value returned by wait */
  char           *v[NV];	/* array of pointers to command line tokens */
  char           *sep = " \t\n";/* command line token separators    */
  int             i;		/* parse index */
  int             cwd[1024];
  int num; //no of tokens in command line

  /* prompt for and process one command line at a time  */

  while (1) {			/* do Forever */
    prompt();
    fgets(line, NL, stdin);
    fflush(stdin);

    if (feof(stdin)) {		/* non-zero on EOF  */

      fprintf(stderr, "EOF pid %d feof %d ferror %d\n", getpid(),
	      feof(stdin), ferror(stdin));
      exit(0);
    }
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
      continue;			/* to prompt */

    v[0] = strtok(line, sep);
    num = 1;
    while (num < NV && v[num-1] != NULL){
        v[num] = strtok(NULL,sep);
        num++;
    }
 if (strcmp(v[0], "cd") == 0) {
            /* change the current working directory */
            if (num > 1) {
                if (chdir(v[1]) != 0) {
                    perror("chdir");
                }
            } else {
                /* go to the home directory */
                if (chdir(getenv("HOME")) != 0) {
                    perror("chdir");
                }
            }
            continue;
        }
    /* assert i is number of tokens + 1 */

    /* fork a child process to exec the command in v[0] */

    switch (frkRtnVal = fork()) {
    case -1:			/* fork returns error to parent process */
        {
        perror("fork");
        break;
        }
    case 0:			/* code executed only by child process */
        {
        //check for & in last command 
        if (strcmp(v[num-2],"&")==0){
            v[num-2] = NULL;
            num--;

        }
        execvp(v[0], v);
        perror("execvp fail");
        exit(1);
        }
    default:			/* code executed only by parent process */
        {
        if (strcmp(v[num-2],"&")!=NULL){
            wpid = wait(NULL);
            printf("%s done \n", v[0]);
        }else {
            printf("[%d] %s\n",frkRtnVal, v[0]);
        }
        break;
        }
    }				/* switch */
  }				/* while */
  return 0;
}				/* main */
