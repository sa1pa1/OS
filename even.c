#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

  void signals(int signal){
        if(signal == SIGHUP){
            printf("Ouch!\n");
        }else if(signal == SIGINT){
            printf("Yeah!\n");
        }
    }
int main(){
    int n;
    scanf("%d",&n);
    int nn = n;
   if (nn<=0){
    fprintf(stderr, "Enter a positive integer\n");
    return 1;
   }
     struct sigaction act;
     act.sa_handler= signals;
     act.sa_flags = 0; // or SA_RESTART

    sigemptyset(&act.sa_mask);
    if (sigaction(SIGHUP, &act, NULL) == -1) {
        perror("Error handling SIGHUP");
        return 1;
    }

    if (sigaction(SIGINT, &act, NULL) == -1) {
        perror("Error handling SIGINT");
        return 1;
    }


   for ( int i =0; i < nn;i++){
    printf("%d\n",i*2);
    sleep(5);
   }

return 0;

}