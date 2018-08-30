/* Name: Nilabja Bhattacharya Roll: 2018201036 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

#include "create_file.h"

int create_file(char *filename, char *path_for_file){
    strcat(path_for_file, "/");
    strcat(path_for_file,filename);
    return open(path_for_file, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
}