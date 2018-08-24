#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include "rename_file.h"

int rename_file(char *old_name_of_file, char *new_name_of_file){
    int status;
    status = rename(old_name_of_file,new_name_of_file);
    return status==0? 1:0;
}
