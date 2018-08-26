#include "snapshot_directory.h"
#include "copy_block.h"
#include "create_file.h"
#include "create_directory.h"
#include "copy_files_inside_directory.h"
#include "copy_directory.h" 
#include "delete_directory.h" 
#include "delete_file.h" 
#include "goto_directory.h"
#include "list_directory.h"
#include "make_directory.h"
#include "make_source_directory.h"
#include "move_directory.h"
#include "move_file.h"
#include "rename_file.h"
#include "search_file_or_directory.h"
#include "snapshot_directory.h" 
#include "trim_pathname.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    //char filename1[500] = "/home/neil/Documents/OS_Project";
    char filename2[500] = "/home/neil/Downloads/ex-050325";
    char filename3[500] = "/home/neil/Downloads/hello";
    copy_directory(filename2,filename3);
    return 0;
}
