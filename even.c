#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

  void handle_sigint(int signal){
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
    signal(SIGINT, handle_sigint);
    signal(SIGHUP, handle_sigint);

    


   for ( int i =0; i < nn;i++){
    printf("%d\n",i*2);
    sleep(5);
   }

return 0;

}