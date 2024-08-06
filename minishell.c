#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define NV 20            /* max number of command tokens */
#define NL 100            /* input buffer size */
char line[NL];    /* command input buffer */


/*
    shell prompt
*/

void prompt(void)
{
    fprintf(stdout, "\n msh> ");
    fflush(stdout);
}

int main(int argc, char *argv[], char *envp[])
/* argc - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */

{
    int frkRtnVal;    /* value returned by fork sys call */
    int wpid;        /* value returned by wait */
    char *v[NV];    /* array of pointers to command line tokens */
    char *sep = " \t\n";/* command line token separators */
    int i;        /* parse index */
    char cwd[1024]; /* current working directory */
    int numTokens; /* number of tokens in command line */

    /* prompt for and process one command line at a time */

    while (1) {            /* do Forever */
        prompt();
        fgets(line, NL, stdin);
        fflush(stdin);

        if (feof(stdin)) {        /* non-zero on EOF */
            fprintf(stderr, "EOF pid %d feof %d ferror %d\n", getpid(),
             feof(stdin), ferror(stdin));
            exit(0);
        }
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
            continue;            /* to prompt */

        /* tokenize the command line */
        v[0] = strtok(line, sep);
        numTokens = 1;
        while (numTokens < NV && v[numTokens-1] != NULL) {
            v[numTokens] = strtok(NULL, sep);
            numTokens++;
        }

        /* handle built-in commands */
        if (strcmp(v[0], "cd") == 0) {
            /* change the current working directory */
            if (numTokens > 1) {
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

        /* fork a child process to exec the command in v[0] */
        switch (frkRtnVal = fork()) {
            case -1:            /* fork returns error to parent process */
                perror("fork");
                break;
            case 0:            /* code executed only by child process */
                /* check if the last token is "&" */
                if (strcmp(v[numTokens-2], "&") == 0) {
                    /* put the command in background */
                    v[numTokens-2] = NULL;
                    numTokens--;
                }
                /* execute the command */
                execvp(v[0], v);
                /* if execvp fails, terminate the child process */
                perror("execvp");
                exit(1);
            default:            /* code executed only by parent process */
                if (strcmp(v[numTokens-2], "&") != 0) {
                    /* wait for the child process to finish */
                    wpid = wait(NULL);
                    printf("%s done \n", v[0]);
                } else {
                    /* report that the command is in background */
                    printf("[%d] %s\n", frkRtnVal, v[0]);
                }
                break;
        }                /* switch */
    }                /* while */
    return 0;
}                /* main */