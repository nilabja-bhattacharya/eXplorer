#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int make_directory(char *path){
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
        return 1;
    }   
    return 0;
}
int main(){
    int i =make_directory("/home/neil/Downloads/hello/ilove");
    exit(0);
}
