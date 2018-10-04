//Kill
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int pid;
    if(argc != 2){
        printf("kill [PID]\n");
        exit(1);
    }
    sscanf(argv[1], "%d", &pid);
    kill(pid, SIGTERM);
}
