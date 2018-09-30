#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>

void ls(char *dirname);
void subls(char *dirname);

void lsO(char *dirname, char *info);
char *uis_to_name(uid_t uid);
char *gid_to_name(gid_t gid);


void mode( int mode, char str[] ){
    strcpy( str, "----------" );
    if ( S_ISLNK(mode)) str[0] = 'l';
    if ( S_ISFIFO(mode)) str[0] = 'p';
    if ( S_ISSOCK(mode)) str[0] = 's';
    if ( S_ISDIR(mode) ) str[0] = 'd';
    if ( S_ISCHR(mode) ) str[0] = 'c';
    if ( S_ISBLK(mode) ) str[0] = 'b';
    if ( mode & S_IRUSR ) str[1] = 'r';
    if ( mode & S_IWUSR ) str[2] = 'w';
    if ( mode & S_IXUSR ) str[3] = 'x';
    if ( mode & S_IRGRP ) str[4] = 'r';
    if ( mode & S_IWGRP ) str[5] = 'w';
    if ( mode & S_IXGRP ) str[6] = 'x';
    if ( mode & S_IROTH ) str[7] = 'r';
    if ( mode & S_IWOTH ) str[8] = 'w';
    if ( mode & S_IXOTH ) str[9] = 'x';
}

void ls( char *dirname){
    DIR *df;
    struct dirent *direntp;
    void mode();
    struct stat info;
    char *uid_to_name(), *ctime(), *gid_to_name();
    char modestr[11];

    if ( ( df = opendir( dirname ) ) == NULL ){
        printf("Cannot open %s\n", dirname);
        exit(1);
    }

    chdir(dirname);

    while ( ( direntp = readdir( df ) ) != NULL ){
        if(lstat(direntp->d_name, &info) == -1)
            perror(direntp->d_name);

            mode( info.st_mode, modestr );
            printf( "%s" , modestr );
            printf( "%4d " , (int) info.st_nlink);
            printf( "%s " , uid_to_name(info.st_uid) );
            printf( "%-s" , gid_to_name(info.st_gid) );
            printf( "%6ld " , (long)info.st_size);
            printf( "%.12s ", 4+ctime(&info.st_mtime));
            printf( "%s\n" , direntp->d_name);

            if((modestr[0] =='d' && strcmp(direntp->d_name, ".") && strcmp(dir    entp->d_name,".."))){
                subls(direntp->d_name);
            }
    }
    chdir("..");
}

void subls( char *dirname){
    DIR *df;
    struct dirent *direntp;
    void mode();
    struct stat info;

    char *uid_to_name(), *ctime(), *gid_to_name();
    char modestr[11];

    if ( ( df = opendir( dirname ) ) == NULL ){
        printf("Cannot open %s\n", dirname);
        exit(1);
    }

    chdir(dirname);

    while ( ( direntp = readdir( df ) ) != NULL ){
        if(lstat(direntp->d_name, &info) == -1)
            perror(direntp->d_name);

            mode( info.st_mode, modestr );
            printf( "\t%s" , modestr );
            printf( "%4d " , (int) info.st_nlink);
            printf( "%s " , uid_to_name(info.st_uid) );
            printf( "%-s" , gid_to_name(info.st_gid) );
            printf( "%6ld " , (long)info.st_size);
            printf( "%.12s ", 4+ctime(&info.st_mtime));
            printf( "%s\n" , direntp->d_name);

            if((modestr[0] =='d' && strcmp(direntp->d_name, ".") && strcmp(dir    entp->d_name,".."))){
                 subls(direntp->d_name);
             }
    }
    chdir("..");
}

void lsO(char *dirname, char *pa){
    DIR *df;
    struct dirent *direntp;
    int fd, i;
    void mode();
    struct stat info;

    char *uid_to_name(), *ctime(), *gid_to_name();
    char modestr[11], size[20], time[10];
    char* tempU, *tempG;

    if((fd =open(pa, O_RDWR|O_CREAT|O_TRUNC,S_IRWXU))<0){
        chdir(dirname);
        ls(dirname);
        exit(1);
    }
    if ( ( df = opendir( dirname ) ) == NULL ){
        printf("Cannot open %s\n", dirname);
        exit(1);
    }
    chdir(dirname);

    while ( ( direntp = readdir( df ) ) != NULL ){
        if(lstat(direntp->d_name, &info) == -1)
            perror(direntp->d_name);

            mode( info.st_mode, modestr );
            write(4, modestr, 11);

            tempU = uid_to_name(info.st_uid);
            write(4, tempU, strlen(tempU));
            write(4," ", 1);

            tempG= gid_to_name(info.st_gid);
            write(4, tempG, strlen(tempG));
            write(4, " ", 1);

            sprintf(size,"%d", info.st_size);
            write(4, size, strlen(size));
            write(4, " ", 1);

            write(4, " ", 1);
            write(4, direntp->d_name, strlen(direntp->d_name));
            write(4, " ", 1);
            write(4, 4+ctime(&info.st_mtime), strlen(4+ctime(&info.st_mtime)))    ;

            if((modestr[0] =='d' && strcmp(direntp->d_name, ".") && strcmp(dir    entp->d_name,".."))){
                lsO(direntp->d_name,pa);
            }
    }
    chdir("..");
}

char *uid_to_name( uid_t uid ){
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];
    if ( ( pw_ptr = getpwuid( uid ) ) == NULL ){
        sprintf(numstr,"%d", uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name ;
}


char *gid_to_name( gid_t gid ) {
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];
    if ( ( grp_ptr = getgrgid(gid) ) == NULL ){
        sprintf(numstr,"%d", gid);
        return numstr;
    }
    else
        return grp_ptr->gr_name;
}

main(int argc, char *argv[]){

    int optd=0; int opto=0;
    int indexd, inD, indexo, inO, j, i =1;

    while(argv[i]){

        if(argv[i][0] == '-'){
            for(j=1;j<strlen(argv[i]);j++){
                switch(argv[i][j]){

                    case 'd' :
                        optd=1;
                        indexd =i;
                        break;

                    case 'o':
                        opto=1;
                        indexo =i;
                        break;

                    default:
                        break;
                }
            }
        }
        i++;
    }

    // - d - o
    if(optd&&opto)
        lsO(argv[indexd+1], argv[indexo+1]);
    // - d
    else if(optd)
        ls(argv[indexd+1]);
    // - o
    else if(opto)
            lsO(".", argv[indexo+1]);
    else if (argc == 1)
        ls( "." );
    else
        while ( --argc ){
            printf("%s\n", *++argv );
            ls( *argv );
        }
}
