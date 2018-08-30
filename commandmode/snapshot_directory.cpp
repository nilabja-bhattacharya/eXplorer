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

#include "snapshot_directory.h"

void snapshot_directory(char *path_name_for_directory_to_be_traversed, char *root_path_name){
    DIR *pointer_to_path_name_for_directory_to_be_printed;
    DIR *pointer_to_path_name_for_directory_to_be_traversed;
    struct dirent *entry;
    struct stat statbuf;

    if((pointer_to_path_name_for_directory_to_be_printed = opendir(path_name_for_directory_to_be_traversed)) == NULL || (pointer_to_path_name_for_directory_to_be_traversed = opendir(path_name_for_directory_to_be_traversed)) == NULL){
        fprintf(stderr, "cannot open directory: %s\n", path_name_for_directory_to_be_traversed);
        return;
    }
    chdir(path_name_for_directory_to_be_traversed);
    int i = strlen(root_path_name);
    printf(".");
    for(;i<strlen(path_name_for_directory_to_be_traversed);i++){
        printf("%c",path_name_for_directory_to_be_traversed[i]);
    }
    printf(":\n");
    while((entry=readdir(pointer_to_path_name_for_directory_to_be_printed))!=NULL){
        lstat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)){
            if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;
            printf("%10s\t ",entry->d_name);
        }
        else
            printf("%10s\t ",entry->d_name);
    }
    printf("\n \n");
    while((entry=readdir(pointer_to_path_name_for_directory_to_be_traversed))!=NULL){
        char *buffer;
        if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;
        size_t path_length_of_directory = strlen(path_name_for_directory_to_be_traversed) + strlen(entry->d_name) + 2;
        buffer = (char *)malloc(path_length_of_directory);
        snprintf(buffer, path_length_of_directory, "%s/%s", path_name_for_directory_to_be_traversed, entry->d_name);
        lstat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)){
            snapshot_directory(buffer, root_path_name);
            //printf("%s", buffer);
        }
    }
    chdir("..");
    closedir(pointer_to_path_name_for_directory_to_be_traversed);
}