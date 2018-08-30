/* Name: Nilabja Bhattacharya Roll: 2018201036 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

#include "make_directory.h"

int make_directory(char *path_name_for_directory){
    struct stat status_of_directory = {0};
    if (stat(path_name_for_directory, &status_of_directory) == -1) {
        mkdir(path_name_for_directory, 0700);
        return 1;
    }   
    return 0;
}