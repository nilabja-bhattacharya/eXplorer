/*
This file implements the copy of file in block sizes of 4096 from source file to destination file
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

#include "copy_block.h"
#include "trim_pathname.h"

#define BLOCK_SIZE 4096
 
int copy_block(char *path_of_file_name_1, char *path_of_file_name_2){
    char block[BLOCK_SIZE];
    int input_file, output_file;
    int read_data;
    trim_pathname(path_of_file_name_1,path_of_file_name_2);
    input_file = open(path_of_file_name_1,O_RDONLY);
    output_file = open(path_of_file_name_2, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
    struct stat status_of_file;
    stat(path_of_file_name_1, &status_of_file);
    chmod(path_of_file_name_2, status_of_file.st_mode);
    while((read_data=read(input_file, block, sizeof(block)))>0)
        write(output_file, block, read_data);
    return 0;
}