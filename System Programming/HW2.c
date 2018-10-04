HW2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#define MAXLINE 100
#define MAXARG 20

int childs[3];
void mychild(int s){
    pid_t pid;
    int status;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0){
        int i;
        if(!WIFEXITED(status) && !WIFSIGNALED(status))
            continue;
        for(i = 0; i < 3; i++){
            if(childs[i] == pid){
                childs[i] = -1;
            }
        }
    }
    if(childs[0] == -1 && childs[1] == -1 && childs[2] == -1){
        printf("All child process terminated\n");
        exit(0);
    }
}

int main(){

    int i;
    signal(SIGCHLD, mychild);

    for( i = 0; i < 3; i++ )
        if(!(childs[i] = fork()))
            execlp("sample", "sample", NULL);
    while(1){
        char line[MAXLINE];
        int size;
        char *token;
        int argc = 0;
        char *argv[MAXARG] = {NULL};
        pid_t childpid;

        printf("$ ");
        fgets(line, 100, stdin);
        size=strlen(line);

        if(line[size-1]=='\n'){
            line[size-1]='\0';
            size--;
        }

        if(!size)
            continue;
        token = strtok(line, " ");

        while(token != NULL){
            argv[argc++] = strdup(token);
            token = strtok(NULL, " ");
        }

        if( !(childpid = fork()) ){
            char filename[100] = "./";
            strcat(filename, argv[0]);
            execvp(filename, argv);
            perror("execlp error");
            exit(1);
        }
        else if(childpid != -1){
            int status;
            waitpid(childpid, &status, 0);
        }
    }

    return 0;
}
