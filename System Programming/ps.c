//Ps
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

int main(){
    pid_t shellpid = getppid();
    DIR *procdir = opendir("/proc");
    struct dirent *subfile;

    printf("Process\nPID\tPPID\tNAME\tSTATE\n");
 
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
 
        if(ppid != shellpid)
            continue;

        printf("%d\t%d\t%s\t%  c\n", pid, ppid, filename, state);
    }
    closedir(procdir);
}
