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

#include "search_file_or_directory.h"

void search_file_or_directory(char *path_name_for_current_directory, char *file_or_folder_name, char *root_path_name){
    DIR *pointer_to_path_name_current_directory;
    struct dirent *entry;
    struct stat statbuf;

    if((pointer_to_path_name_current_directory = opendir(path_name_for_current_directory)) == NULL){
        fprintf(stderr, "cannot open directory: %s\n", path_name_for_current_directory);
        return;
    }
    chdir(path_name_for_current_directory);
    while((entry=readdir(pointer_to_path_name_current_directory))!=NULL){
        char *buffer;
        size_t path_length_of_directory = strlen(path_name_for_current_directory) + strlen(entry->d_name) + 2;
        buffer = (char *)malloc(path_length_of_directory);
        snprintf(buffer, path_length_of_directory, "%s/%s", path_name_for_current_directory, entry->d_name);
        if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;
        if(!strcmp(file_or_folder_name, entry->d_name)){
            int i = strlen(root_path_name);
            printf(".");
            for(;i<strlen(buffer);i++)
                printf("%c",buffer[i]);
            printf("\n\n");
        }
        lstat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)){
            search_file_or_directory(buffer, file_or_folder_name,root_path_name);
            //printf("%s", buffer);
        }
    }
    chdir("..");
    closedir(pointer_to_path_name_current_directory);
}