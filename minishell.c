
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
  // int wpid;            /* value returned by wait */
  char *v[NV];         /* array of pointers to command line tokens */
  char *sep = " \t\n"; /* command line token separators */
  int i;               /* parse index */

  int bg = 0;          /*background process flag*/
  int bg_PID[NV];
  char *bg_cmds[NV];
  char *bg_cmds_value[NV]; 


  memset(bg_PID, 0, sizeof(bg_PID));
  memset(bg_cmds, 0, sizeof(bg_cmds));
  memset(bg_cmds_value,0,sizeof(bg_cmds_value));


  /* prompt for and process one command line at a time */
  while (1)
  { /* do Forever */
    int status;
    pid_t finished_pid;
        while ((finished_pid = waitpid(-1, &status, WNOHANG)) > 0) {
            for (int i = 1; i < bg_counter;i++) {
                if (bg_PID[i] == finished_pid) {
                   printf("[%d]+ Done %s %s\n", i, bg_cmds[i], bg_cmds_value[i]);
                   fflush(stdout);

                   
                    free(bg_cmds[i]); // Free allocated memory
                    bg_PID[i] = 0;
                    bg_cmds_value[i] = NULL;
                    bg_cmds[i] = NULL;
                    break;
                }
            }
        
    }


    prompt();
    fgets(line, NL, stdin);
    fflush(stdin);

    if (feof(stdin)){ /* non-zero on EOF */
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
  // check for & to flag as background process 
  if (i>1 && strcmp(v[i-1], "&")==0){
    bg = 1;
    v[i-1] = NULL; //remove & as token
  }   else {bg=0;}

  //HANDLING CD
   if (strcmp(v[0], "cd") == 0)
        {
            const char *homeDir = getenv("HOME");
            if (v[1] == NULL)
            {
                if (homeDir == NULL)
                {
                    fprintf(stderr, "msh: HOME not set\n");
                }
                else if (chdir(homeDir) != 0)
                {
                    perror("msh: chdir");
                }
            }
            else
            {
                if (chdir(v[1]) != 0)
                {
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
        
  
      if (bg) {
                    bg_PID[bg_counter] = frkRtnVal;
                    bg_cmds[bg_counter] = strdup(v[0]);
                    if (v[1]!=NULL){
                        bg_cmds_value[bg_counter] = strdup(v[1]);
                    }else {
                        bg_cmds_value[bg_counter] = NULL;
                    }
                    printf("[%d] %d\n", bg_counter, frkRtnVal);
                    fflush(stdout);
                    bg_counter++;
                } else {
                    if (waitpid(frkRtnVal, NULL, 0) == -1) {
                        perror("msh: wait");
                    }
                
                break;
        }

        } /* switch */
      
  } /* while */
} /* main */
