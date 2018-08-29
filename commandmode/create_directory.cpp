#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

#include "create_directory.h"

int create_directory(char *directory_name, char *path_for_directory){
    struct stat status = {0};
    strcat(path_for_directory, "/");
    strcat(path_for_directory,directory_name);
    if (stat(path_for_directory, &status) == -1) {
        mkdir(path_for_directory, 0700);
        return 1;
    }   
    return 0;
}