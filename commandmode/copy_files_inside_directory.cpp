#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

#include "copy_directory.h"
#include "make_directory.h"
#include "trim_pathname.h"
#include "copy_block.h"

int copy_files_inside_directory(char *path_name_for_directory_to_be_copied, char *path_to_new_directory){
    DIR *pointer_path_for_directory_to_be_copied;
    DIR *pointer_path_to_new_directory;
    size_t path_length_for_path_name_for_directory_to_be_copied = strlen(path_name_for_directory_to_be_copied);
    size_t path_length_path_to_new_directory = strlen(path_to_new_directory);
    int return_status = -1;
    if((pointer_path_for_directory_to_be_copied = opendir(path_name_for_directory_to_be_copied)) == NULL || (pointer_path_to_new_directory = opendir(path_to_new_directory))==NULL)
        return 0;
    chdir(path_name_for_directory_to_be_copied);
    chdir(path_to_new_directory);
    if(pointer_path_for_directory_to_be_copied && pointer_path_to_new_directory){
        struct dirent *pointer_to_files_and_folder;
        return_status = 0;
        while(!return_status && (pointer_to_files_and_folder=readdir(pointer_path_for_directory_to_be_copied))){
            int return_status_of_files_inside_folder = -1;
            char buffer1[1000];
            char buffer2[1000];
            size_t length_of_pointer_to_file_or_folder;
            size_t length_of_pointer_to_new_file_or_directory;
            if(!strcmp(".",pointer_to_files_and_folder->d_name) || !strcmp("..", pointer_to_files_and_folder->d_name))
                continue;
            length_of_pointer_to_file_or_folder = path_length_for_path_name_for_directory_to_be_copied + strlen(pointer_to_files_and_folder->d_name) + 2;
            length_of_pointer_to_new_file_or_directory = path_length_path_to_new_directory + strlen(pointer_to_files_and_folder->d_name) + 2;
            //buffer1 = (char *)malloc(length_of_pointer_to_file_or_folder);
            //buffer2 = (char *)malloc(length_of_pointer_to_new_file_or_directory);
            if(buffer1 && buffer2){
                struct stat statbuffer;
                snprintf(buffer1, length_of_pointer_to_file_or_folder, "%s/%s", path_name_for_directory_to_be_copied, pointer_to_files_and_folder->d_name);
                snprintf(buffer2,length_of_pointer_to_new_file_or_directory,"%s/%s", path_to_new_directory, pointer_to_files_and_folder->d_name);
                if(!stat(buffer1, &statbuffer)){
                    if(S_ISDIR(statbuffer.st_mode)){
                        if(make_directory(buffer2)){
                            struct stat status_of_file_or_folder;
                            return_status_of_files_inside_folder = copy_directory(buffer1,buffer2);
                            stat(buffer1, &status_of_file_or_folder);
                            chmod(buffer2, status_of_file_or_folder.st_mode);
                        }
                        else
                            return 0;
                            //printf("Error in making directory\n");
                    }
                    else{
                        buffer2[path_length_path_to_new_directory]='\0';
                        return_status_of_files_inside_folder = copy_block(buffer1,buffer2);
                    }
                }
            }
            return_status = return_status_of_files_inside_folder;
        }
        chdir("..");
        closedir(pointer_path_for_directory_to_be_copied);
        return return_status;
    }
    //printf("%s %s",path_name_for_directory_to_be_copied, path_to_new_directory);
}
