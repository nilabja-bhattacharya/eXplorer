#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "delete_file.h"

int  delete_file(char *path_of_file){
    return remove(path_of_file);  
}
