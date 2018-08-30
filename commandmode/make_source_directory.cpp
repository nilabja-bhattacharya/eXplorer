/* Name: Nilabja Bhattacharya Roll: 2018201036 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

#include "make_directory.h"
#include "trim_pathname.h"

void make_source_directory(char *path_name_for_directory_to_be_copied, char *path_to_new_directory){
    trim_pathname(path_name_for_directory_to_be_copied, path_to_new_directory);
    make_directory(path_to_new_directory);
}