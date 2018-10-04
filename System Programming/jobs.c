//Jobs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

int checknumber(char *str){
    while(*str != '\0'){
        if(!isdigit(*str)){
            return 0;
        }
        str++;
    }
    return 1;
}

void printjobs(pid_t wantppid, char *wantstates){
    DIR *procdir = opendir("/proc");
    struct dirent *subfile;

    while((subfile = readdir(procdir)) != NULL){
        char path[300], filename[100], state;
        FILE *file;
        int pid, ppid;

        if(subfile->d_type != DT_DIR)
            continue;
        if(!checknumber(subfile->d_name))
            continue;

        strcpy(path, "/proc/");
        strcat(path, subfile->d_name);
        strcat(path, "/stat");

        file=fopen(path,"r");
        if(!file)
            continue;
        if(fscanf(file, "%d (%[^)]) %c %d", &pid, filename, &state, &ppid) < 4    ){
            fclose(file);
            continue;
        }
        fclose(file);

        if(ppid != wantppid)
            continue;
        if(!strchr(wantstates, state))
            continue;

        printf("%d\t%d\t%s\t  %c\n", pid, ppid, filename, state);
    }
    closedir(procdir);
}

int main(){
    pid_t shellpid = getppid();
    printf("Running jobs:\n");
    printf("PID\tPPID\tNAME\tSTATE\t\n");
    printjobs(shellpid, "RSDW");
    printf("\nStopped jobs:\n");
    printjobs(shellpid, "T");
}
