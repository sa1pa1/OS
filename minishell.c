
/*********************************************************************
Program : miniShell Version : 1.3
--------------------------------------------------------------------
skeleton code for linix/unix/minix command line interpreter
--------------------------------------------------------------------
File : minishell.c
Compiler/System : gcc/linux
********************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#define NV 20      /* max number of command tokens */
#define NL 100     /* input buffer size */
    char line[NL]; /* command input buffer */
    int bg_counter = 1;
/*
shell prompt
*/
void prompt(void)
{
  fflush(stdout);
}

int main(int argk, char *argv[], char *envp[])
/* argk - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */
{
  int frkRtnVal;       /* value returned by fork sys call */
  int wpid;            /* value returned by wait */
  char *v[NV];         /* array of pointers to command line tokens */
  char *sep = " \t\n"; /* command line token separators */
  int i;               /* parse index */
  int bg = 0;          /*flag bavkground process*/
  int bg_pids[NV];     /* array to store background PIDs */
  char *bg_cmds[NV];   /* array to store background commands */
  int bg_indices[NV];  /* array to store background indices */
  /* prompt for and process one command line at a time */

 /* Initialize background process arrays */
    memset(bg_pids, 0, sizeof(bg_pids));
    memset(bg_cmds, 0, sizeof(bg_cmds));
    memset(bg_indices, 0, sizeof(bg_indices));

  while (1)
  { /* do Forever */
    prompt();
    fgets(line, NL, stdin);
    fflush(stdin);
    if (feof(stdin))
    { /* non-zero on EOF */
      exit(0);
    }
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
      continue; /* to prompt */
    v[0] = strtok(line, sep);
    for (i = 1; i < NV; i++)
    {
      v[i] = strtok(NULL, sep);
      if (v[i] == NULL)
        break;
    }
  //check for & to flag as background process
  if (i >1 && strcmp(v[i-1],"&")==0){
    bg = 1;        //flag bg process
    v[i-1] = NULL; //remove the & from token list
  }
  else {
    bg = 0;
  }
  //HANDLING CD
   if (strcmp(v[0], "cd") == 0){
            const char *homeDir = getenv("HOME");
            if (v[1] == NULL){
                if (homeDir == NULL){
                    fprintf(stderr, "msh: HOME not set\n");
                }
                else if (chdir(homeDir) != 0){
                    perror("msh: chdir");
                }
            }
            else{
                if (chdir(v[1]) != 0){
                    perror("msh: chdir");
                }
            }
            continue; /* skip the fork/exec for "cd" */
        }
    /* assert i is number of tokens + 1 */
    /* fork a child process to exec the command in v[0] */
     /* fork a child process to exec the command in v[0] */
        switch (frkRtnVal = fork())
        {
        case -1: /* fork returns error to parent process */
        {
            perror("msh: fork");
            break;
        }
        case 0: /* code executed only by child process */
        {
            if (execvp(v[0], v) == -1)
            {
                perror("msh: execvp");
            }
            exit(EXIT_FAILURE); /* exit if execvp fails */
        }
        default: /* code executed only by parent process */
        {
    if (!bg){
                if ((wpid = wait(0)) == -1)
                {
                    perror("msh: wait");
                }
            //remove printing of done t align with gradescope
            break;
        }
        
         if (bg) {
                // Track background process
                bg_pids[bg_counter] = frkRtnVal;
                bg_cmds[bg_counter] = strdup(v[0]);
                bg_indices[bg_counter] = bg_counter;
                printf("[%d] %d\n", bg_counter, frkRtnVal);
                fflush(stdout);
                bg_counter++;
            } else {
                if ((wpid = wait(0)) == -1) {
                    perror("msh: wait");
                }
            }
            break;

             // Check for completed background processes
        
        
        }
        } /* switch */
        int status;
        pid_t finished_pid;
        while ((finished_pid = waitpid(-1, &status, WNOHANG)) > 0) {
            for (i = 1; i < bg_counter; i++) {
                if (bg_pids[i] == finished_pid) {
                   printf("[%d]+ Done %s %s\n", i, bg_cmds[i], v[1]);
                    free(bg_cmds[i]); // Free allocated memory
                    bg_pids[i] = 0;
                    bg_cmds[i] = NULL;
                    break;
                }
            }
        
    }
  } /* while */
} /* main */
