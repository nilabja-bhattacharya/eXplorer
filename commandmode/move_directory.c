#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "copy_directory.h"
#include "make_directory.h"
#include "make_source_directory.h"
#include "trim_pathname.h"
#include "copy_block.h"
#include "copy_files_inside_directory.h"
#include "delete_directory.h"

int move_directory(char *path_name_for_directory_to_be_moved, char *path_to_new_directory){
    if(copy_directory(path_name_for_directory_to_be_moved, path_to_new_directory))
        return delete_directory(path_name_for_directory_to_be_moved);
    return 0;
}