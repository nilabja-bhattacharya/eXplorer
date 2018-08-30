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

#include "delete_directory.h"

int delete_directory(char *path_of_directory){
    DIR *pointer_to_path;
    size_t length_of_path = strlen(path_of_directory);
    int return_status = -1;
    if((pointer_to_path = opendir(path_of_directory)) == NULL){
        fprintf(stdout, "cannot open directory: %s\n", path_of_directory);
        return 0;
    }
    chdir(path_of_directory);
    if(pointer_to_path){
        struct dirent *pointer_to_files;
        return_status=0;
        while(!return_status && (pointer_to_files=readdir(pointer_to_path))){
            int return_status_inside_folder = -1;
            char *buffer;
            size_t len;
            if(!strcmp(pointer_to_files->d_name,".") || !strcmp(pointer_to_files->d_name,".."))
                continue;
            len = length_of_path + strlen(pointer_to_files->d_name)+2;
            buffer = (char *)malloc(len);

            if(buffer){
                struct stat statbuffer;
                snprintf(buffer, len, "%s/%s", path_of_directory, pointer_to_files->d_name);
                if(!stat(buffer, &statbuffer)){
                    if(S_ISDIR(statbuffer.st_mode))
                        return_status_inside_folder = delete_directory(buffer);
                    else
                        return_status_inside_folder = unlink(buffer);
                }
                free(buffer);
            }
            return_status = return_status_inside_folder;
        }
        chdir("..");
        closedir(pointer_to_path);
        if(!return_status)
            return_status = rmdir(path_of_directory);
        return return_status;
    }
}
