#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sighup(int signal){
            printf("Ouch!\n");
    }

void handle_sigint(int signal){
            printf("Yeah!\n");
    }

int main(int even, char *arg[]){
    int n = atoi(arg[1]);
    if (n<=0){
        fprintf(stderr, "Enter a positive integer\n");
        return 1;
        }   

    signal(SIGINT, handle_sigint);
    signal(SIGHUP, handle_sighup);


    for ( int i =0; i < n;i++){
        printf("%d\n",i*2);
        sleep(5);
    }

return 0;

}