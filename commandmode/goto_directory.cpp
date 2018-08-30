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

#include "goto_directory.h"
#include "search_file_or_directory.h"

int goto_directory(char *path_name_for_current_directory, char *directory_name){
    DIR *pointer_to_path_name_current_directory;
    struct dirent *entry;
    struct stat statbuf;

    if((pointer_to_path_name_current_directory = opendir(path_name_for_current_directory)) == NULL){
        //fprintf(stderr, "cannot open directory: %s\n", path_name_for_current_directory);
        return 0;
    }
    chdir(path_name_for_current_directory);
    while((entry=readdir(pointer_to_path_name_current_directory))!=NULL){
        char *buffer;
        size_t path_length_of_directory = strlen(path_name_for_current_directory) + strlen(entry->d_name) + 2;
        buffer = (char *)malloc(path_length_of_directory);
        snprintf(buffer, path_length_of_directory, "%s/%s", path_name_for_current_directory, entry->d_name);
        if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;
        lstat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)){
            if(!strcmp(directory_name, entry->d_name)){
                closedir(pointer_to_path_name_current_directory);
                return 1;
            }
            search_file_or_directory(buffer, directory_name,path_name_for_current_directory);
            //printf("%s", buffer);
        }
        else{
            if(!strcmp(directory_name, entry->d_name)){
                char *s=(char * )malloc(sizeof(buffer) + 100);
                //snprintf(s, sizeof(buffer)+100, "%s %s", "xdg-open", buffer);
                int pid = fork();
                if (pid == 0) {
                    execl("/usr/bin/xdg-open", "xdg-open", buffer, (char *)0);
                    exit(1);
                }
                //system(s);
                closedir(pointer_to_path_name_current_directory);
                return 2;
            }
        }
    }
    chdir("..");
    closedir(pointer_to_path_name_current_directory);
    return 0;
}