#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
//testing gradeschope not MINE!!!
#define NV 20 /* max number of command tokens */
#define NL 100 /* input buffer size */
char line[NL]; /* command input buffer */

/*
shell prompt
*/
void prompt(void) {
    fprintf(stdout, "\n msh> ");
    fflush(stdout);
}

int main(int argc, char *argv[], char *envp[]) {
    int frkRtnVal; /* value returned by fork sys call */
    char *v[NV]; /* array of pointers to command line tokens */
    char *sep = " \t\n";/* command line token separators */
    int i; /* parse index */

    while (1) { /* do Forever */
        prompt();
        fgets(line, NL, stdin);

        if (feof(stdin)) { /* non-zero on EOF */
            fprintf(stderr, "EOF pid %d feof %d ferror %d\n", getpid(), feof(stdin), ferror(stdin));
            exit(0);
        }

        if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
            continue; /* to prompt */

        v[0] = strtok(line, sep);
        for (i = 1; i < NV; i++) {
            v[i] = strtok(NULL, sep);
            if (v[i] == NULL)
                break;
        }
        /* assert i is number of tokens + 1 */

        if (v[0] != NULL) {
            // Check if the last token ends with '&'
            int background = 0;
            int last_idx = i - 1;
            if (last_idx > 0 && v[last_idx] != NULL) {
                size_t len = strlen(v[last_idx]);
                if (len > 0 && v[last_idx][len - 1] == '&') {
                    background = 1;
                    v[last_idx][len - 1] = '\0'; // Remove the '&' character
                }
            }

            // Handle the "cd" command separately
            if (strcmp(v[0], "cd") == 0) {
                if (i >= 2) {
                    if (chdir(v[1]) != 0) {
                        perror("chdir");
                    }
                } else {
                    fprintf(stderr, "cd: Missing argument\n");
                }
                continue;
            }

            /* fork a child process to exec the command in v[0] */
            switch (frkRtnVal = fork()) {
                case -1: /* fork returns error to parent process */
                    perror("fork");
                    break;
                case 0: /* code executed only by child process */
                    execvp(v[0], v);
                    perror("execvp");
                    exit(1); // Terminate child process on exec failure
                default: /* code executed only by parent process */
                    if (!background) {
                        int status;
                        waitpid(frkRtnVal, &status, 0);
                        printf("%s done \n", v[0]);
                    }
                    break;
            } /* switch */
        }
    } /* while */
    return 0;
} /* main */