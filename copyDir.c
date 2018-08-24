#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


void trimpathname(char *path1, char *path2){
    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);
    int i=0,j=len2;
    for(i=len1-1;i>=0;i--){
        if(*(path1+i)=='/')
            break;
    }
    for(;i<len1;i++,j++)
        *(path2+j)=*(path1+i);
    *(path2+j)='\0';
}
int copy_block(char *path1, char *path2){
    char block[1024];
    int in, out;
    int nread;
    in = open(path1,O_RDONLY);
    out = open(path2, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
    struct stat tmp;
    stat(path1, &tmp);
    chmod(path2, tmp.st_mode);
    while((nread=read(in, block, sizeof(block)))>0)
        write(out, block, nread);
    return 0;
}

int make_directory(char *path){
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
        return 1;
    }   
    return 0;
}

int copy_directory(char *path1, char *path2){
    DIR *dp1, *dp2;;
    size_t path_len1 = strlen(path1);
    size_t path_len2 = strlen(path2);
    int r = -1;
    printf("%s %s\n",path1, path2);
    if((dp1 = opendir(path1)) == NULL || (dp2 = opendir(path2))==NULL){
        printf("cannot open directory: %s\n", path1);
        return 0;
    }
    chdir(path1);
    chdir(path2);
    if(dp1 && dp2){
        struct dirent *p;
        r=0;
        while(!r && (p=readdir(dp1))){
            int r2 = -1;
            char *buffer1;
            char *buffer2;
            size_t len1, len2;
            if(!strcmp(".",p->d_name) || !strcmp("..", p->d_name))
                continue;
            len1 = path_len1 + strlen(p->d_name) + 2;
            len2 = path_len2 + strlen(p->d_name) + 2;
            buffer1 = (char *)malloc(len1);
            buffer2 = (char *)malloc(len2);
            if(buffer1){
                struct stat statbuffer;
                snprintf(buffer1, len1, "%s/%s", path1, p->d_name);
                snprintf(buffer2,len2,"%s/%s", path2, p->d_name);
                printf("%s\n",buffer2);
                if(!stat(buffer1, &statbuffer)){
                    if(S_ISDIR(statbuffer.st_mode)){
                        if(make_directory(buffer2)){
                            struct stat tmp;
                            r2 = copy_directory(buffer1,buffer2);
                            stat(buffer1, &tmp);
                            chmod(buffer2, tmp.st_mode);
                        }
                        else
                            printf("Error in making directory\n");
                    }
                    else
                        r2 = copy_block(buffer1,buffer2);
                }
            }
            r = r2;
        }
        chdir("..");
        closedir(dp1);
        return r;
    }
    printf("%s %s",path1, path2);
}

int main(){
    char path1[100]= "/home/neil/Downloads/ex-050325";
    char path2 [100]= "/home/neil/Downloads/hello/ilove";
    trimpathname(path1,path2);
    printf("%s\n", path2);
    make_directory(path2);
    copy_directory(path1, path2);
    exit(0);
}