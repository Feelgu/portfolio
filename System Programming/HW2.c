HW2
1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <string.h>
  4 #include <sys/types.h>
  5 #include <sys/wait.h>
  6 #include <unistd.h>
  7 #include <signal.h>
  8 #define MAXLINE 100
  9 #define MAXARG 20
 10
 11 int childs[3];
 12
 13 void mychild(int s){
 14     pid_t pid;
 15     int status;
 16     while((pid = waitpid(-1, &status, WNOHANG)) > 0){
 17         int i;
 18         if(!WIFEXITED(status) && !WIFSIGNALED(status))
 19             continue;
 20         for(i = 0; i < 3; i++){
 21             if(childs[i] == pid){
 22                 childs[i] = -1;
 23             }
 24         }
 25     }
 26     if(childs[0] == -1 && childs[1] == -1 && childs[2] == -1){
 27         printf("All child process terminated\n");
 28         exit(0);
 29     }
 30 }
 31
 32 int main(){
 33
 34     int i;
 35     signal(SIGCHLD, mychild);
 36
 37     for( i = 0; i < 3; i++ )
 38         if(!(childs[i] = fork()))
 39             execlp("sample", "sample", NULL);
 40
 41     while(1){
 42         char line[MAXLINE];
 43         int size;
 44         char *token;
45         int argc = 0;
 46         char *argv[MAXARG] = {NULL};
 47         pid_t childpid;
 48
 49         printf("$ ");
 50         fgets(line, 100, stdin);
 51         size=strlen(line);
 52
 53         if(line[size-1]=='\n'){
 54             line[size-1]='\0';
 55             size--;
 56         }
 57
 58         if(!size)
 59             continue;
 60         token = strtok(line, " ");
 61
 62         while(token != NULL){
 63             argv[argc++] = strdup(token);
 64             token = strtok(NULL, " ");
 65         }
 66
 67         if( !(childpid = fork()) ){
 68             char filename[100] = "./";
 69             strcat(filename, argv[0]);
 70             execvp(filename, argv);
 71             perror("execlp error");
 72             exit(1);
 73         }
 74         else if(childpid != -1){
 75             int status;
 76             waitpid(childpid, &status, 0);
 77         }
 78     }
 79
 80     return 0;
 81 }


Ps
1 #include <stdio.h>
  2 #include <string.h>
  3 #include <stdlib.h>
  4 #include <sys/types.h>
  5 #include <unistd.h>
  6 #include <dirent.h>
  7 #include <ctype.h>
  8
  9 int checknumber(char *str){
 10     while(*str != '\0'){
 11         if(!isdigit(*str)){
 12             return 0;
 13         }
 14         str++;
 15     }
 16     return 1;
 17 }
 18
 19 int main(){
 20     pid_t shellpid = getppid();
 21     DIR *procdir = opendir("/proc");
 22     struct dirent *subfile;
 23
 24     printf("Process\nPID\tPPID\tNAME\tSTATE\n");
 25
 26     while((subfile = readdir(procdir)) != NULL){
 27         char path[300], filename[100], state;
 28         FILE *file;
 29         int pid, ppid;
 30
 31         if(subfile->d_type != DT_DIR)
 32             continue;
 33         if(!checknumber(subfile->d_name))
 34             continue;
 35
 36         strcpy(path, "/proc/");
 37         strcat(path, subfile->d_name);
 38         strcat(path, "/stat");
 39
 40         file=fopen(path,"r");
 41         if(!file)
 42             continue;
43         if(fscanf(file, "%d (%[^)]) %c %d", &pid, filename, &state, &ppid) < 4    ){
 44             fclose(file);
 45             continue;
 46         }
 47         fclose(file);
 48
 49         if(ppid != shellpid)
 50             continue;
 51
 52         printf("%d\t%d\t%s\t%  c\n", pid, ppid, filename, state);
 53     }
 54     closedir(procdir);
 55 }


Jobs
1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <string.h>
  4 #include <sys/types.h>
  5 #include <unistd.h>
  6 #include <dirent.h>
  7 #include <ctype.h>
  8
  9 int checknumber(char *str){
 10     while(*str != '\0'){
 11         if(!isdigit(*str)){
 12             return 0;
 13         }
 14         str++;
 15     }
 16     return 1;
 17 }
 18
 19 void printjobs(pid_t wantppid, char *wantstates){
 20     DIR *procdir = opendir("/proc");
 21     struct dirent *subfile;
 22
 23     while((subfile = readdir(procdir)) != NULL){
 24         char path[300], filename[100], state;
 25         FILE *file;
 26         int pid, ppid;
 27
 28         if(subfile->d_type != DT_DIR)
 29             continue;
 30         if(!checknumber(subfile->d_name))
 31             continue;
 32
 33         strcpy(path, "/proc/");
 34         strcat(path, subfile->d_name);
 35         strcat(path, "/stat");
 36
 37         file=fopen(path,"r");
 38         if(!file)
 39             continue;
 40         if(fscanf(file, "%d (%[^)]) %c %d", &pid, filename, &state, &ppid) < 4    ){
 41             fclose(file);
 42             continue;
 43         }
44         fclose(file);
 45
 46         if(ppid != wantppid)
 47             continue;
 48         if(!strchr(wantstates, state))
 49             continue;
 50
 51         printf("%d\t%d\t%s\t  %c\n", pid, ppid, filename, state);
 52     }
 53     closedir(procdir);
 54 }
 55
 56 int main(){
 57     pid_t shellpid = getppid();
 58     printf("Running jobs:\n");
 59     printf("PID\tPPID\tNAME\tSTATE\t\n");
 60     printjobs(shellpid, "RSDW");
 61     printf("\nStopped jobs:\n");
 62     printjobs(shellpid, "T");
 63 }

Kill
1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <signal.h>
  4 #include <unistd.h>
  5
  6 int main(int argc, char *argv[]){
  7     int pid;
  8     if(argc != 2){
  9         printf("kill [PID]\n");
 10         exit(1);
 11     }
 12     sscanf(argv[1], "%d", &pid);
 13     kill(pid, SIGTERM);
 14 }
