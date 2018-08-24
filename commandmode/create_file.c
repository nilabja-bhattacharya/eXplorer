#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "create_file.h"

void create_file(char *filename, char *path_for_file){
    strcat(path_for_file, "/");
    strcat(path_for_file,filename);
}