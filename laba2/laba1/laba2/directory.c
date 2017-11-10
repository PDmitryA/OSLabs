#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAXDIRSIZE 512

int scanfolder(char*, char*);
int diag(char*);
//int insersort(int);

/*
typedef int (*FCMP)(const void*, const void*);
int compare(const void*, const void*);
int compare(const void* s1, const void* s2) {
    return(strcmp((const char*) s1, (const char*) s2));
}
*/
/*
static char* list[512];

int insertsort(int num) {
    int i=0;
    int j;
    char* p;
    while(++i < num) {
      j = i - 1;
      p = list[i];
      while(strcmp(p, list[j]) < 0) {
        list[j+1] = list[j];
        j--;
      }
      list[j+1] = p;
    }
    return(0);
}
*/

int diag(char* mes) {
    write(1, mes, strlen(mes));
    write(1, ": ", 2);
    write(1, sys_errlist[errno], strlen(sys_errlist[errno]));
    write(1, "\n", 1);
    exit(errno);
}

int copyFolderFiles(char* folderFrom, char* folderTo) {
    DIR* fdir;
    struct dirent* file;
    struct stat sbuf[1];

    //printf("folderFrom: %s\n", folderFrom);

    if((fdir = opendir(folderFrom)) == NULL)
        return(errno);

    chdir(folderFrom);
    while((file = readdir(fdir)) != NULL) { 
        char* fnFrom = strcat(strcat(folderFrom, "/"), file->d_name);
    	char* fnTo = strcat(strcat(folderTo, "/"), file->d_name);
        stat(file->d_name, sbuf);
        //printf("file->d_name: %s\n", file->d_name);
        //printf("((sbuf->st_mode) & S_IFMT) : %d\n", ((sbuf->st_mode) & S_IFMT));
        if(((sbuf->st_mode) & S_IFMT) == S_IFREG){
            
            //fprintf(stderr, "fnTo: %s\n", fnTo);
            
            if(access(fnFrom, F_OK)){
                fprintf(stderr, "%s: access denied\n", file->d_name);
                continue;
            }
            //printf("accessed: %s\n", fnFrom);
            if(link(fnFrom, fnTo) == -1){
                fprintf(stderr, "Cannot create file %s\n", fnTo);
                continue;
            }
            //printf("linked: %s\n with %s\n", fnFrom, fnTo);
            if(unlink(fnFrom) == -1){
                fprintf(stderr, "Cannot delete file %s!\n", fnFrom);
                continue;
            }
            //printf("unlinked: %s\n", fnTo);
        }
    }    
    closedir(fdir);
    return(0);
}

int main(int argc, char* argv[]) {
    char currdir[MAXDIRSIZE];
    if(argc < 2)
        exit(-1);
    if(!strcmp(getcwd(currdir, sizeof(currdir)), argv[1]))
        exit(0);
    if(chdir(argv[1]) < 0)
        if(mkdir(argv[1], S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) != 0)
	        return(diag(argv[1]));
    
    copyFolderFiles(currdir, argv[1]);
    return(0);
}

