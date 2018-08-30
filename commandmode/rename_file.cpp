/* Name: Nilabja Bhattacharya Roll: 2018201036 */
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#include "rename_file.h"

int rename_file(char *old_name_of_file, char *new_name_of_file){
    int status;
    status = rename(old_name_of_file,new_name_of_file);
    return status==0? 1:0;
}
