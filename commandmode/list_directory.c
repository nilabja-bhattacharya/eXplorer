#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>

#include "list_directory.h"

void list_directory(char *path_name_for_directory){
    DIR *pointer_to_path_name_for_directory;
    struct dirent *entry;
    struct stat statbuf;

    if((pointer_to_path_name_for_directory = opendir(path_name_for_directory)) == NULL){
        fprintf(stderr, "cannot open directory: %s\n", path_name_for_directory);
        return;
    }
    while((entry=readdir(pointer_to_path_name_for_directory))!=NULL){
        char *buffer1;
        size_t length_of_pointer_to_file_or_folder;
        length_of_pointer_to_file_or_folder = strlen(path_name_for_directory) + strlen(entry->d_name) + 2;
        buffer1 = (char *)malloc(length_of_pointer_to_file_or_folder);
        snprintf(buffer1, length_of_pointer_to_file_or_folder, "%s/%s", path_name_for_directory, entry->d_name);
        lstat(buffer1, &statbuf);
        //printf("%d ",statbuf.st_mode);
        if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;
        if(S_ISDIR(statbuf.st_mode)){
            printf("d");
        }
        else{
            printf("-");
        }
        if(S_IRUSR&statbuf.st_mode){
            printf("r");
        }
        else{
            printf("-");
        }
        if(S_IWUSR&statbuf.st_mode)
            printf("w");
        else
            printf("-");
        if(S_IXUSR&statbuf.st_mode)
            printf("x");
        else
            printf("-");
        if(S_IRGRP&statbuf.st_mode)
            printf("r");
        else
            printf("-");
        if(S_IWGRP&statbuf.st_mode)
            printf("w");
        else
            printf("-");
        if(S_IXGRP&statbuf.st_mode)
            printf("x");
        else
            printf("-");
        if(S_IROTH&statbuf.st_mode)
            printf("r");
        else
            printf("-");
        if(S_IWOTH&statbuf.st_mode)
            printf("w");
        else
            printf("-");
        if(S_IXOTH&statbuf.st_mode)
            printf("x ");
        else
            printf("- ");
        printf("%s ",getpwuid(statbuf.st_uid)->pw_name);
        printf("%s ",getgrgid(statbuf.st_gid)->gr_name);
        printf("%lu ", statbuf.st_size);
        printf("%s\n",entry->d_name);
    }
    chdir("..");
    closedir(pointer_to_path_name_for_directory);
}