#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <iostream>
#include <vector>
using namespace std;

#include "list_directory.h"

struct file_and_folder{
    char *display;
    char *path;
};


void file_or_directory_permission(char *path_name_for_directory, char *file_or_folder_name, char *display){
    struct stat statbuf;
    char *buffer1;
    size_t length_of_pointer_to_file_or_folder;
    length_of_pointer_to_file_or_folder = strlen(path_name_for_directory) + strlen(file_or_folder_name) + 2;
    buffer1 = (char *)malloc(length_of_pointer_to_file_or_folder);
    snprintf(buffer1, length_of_pointer_to_file_or_folder, "%s/%s", path_name_for_directory, file_or_folder_name);
    lstat(buffer1, &statbuf);
    //printf("%d ",statbuf.st_mode);
    if(S_ISDIR(statbuf.st_mode))
        strcat(display,"d");
    else
        strcat(display,"-");
    if(S_IRUSR&statbuf.st_mode)
         strcat(display,"r");
    else
       strcat(display,"-");
    if(S_IWUSR&statbuf.st_mode)
        strcat(display,"w");
    else
        strcat(display,"-");
    if(S_IXUSR&statbuf.st_mode)
        strcat(display,"x");
    else
        strcat(display,"-");
    if(S_IRGRP&statbuf.st_mode)
        strcat(display,"r");
    else
        strcat(display,"-");
    if(S_IWGRP&statbuf.st_mode)
        strcat(display,"w");
    else
        strcat(display,"-");
    if(S_IXGRP&statbuf.st_mode)
        strcat(display,"x");
    else
        strcat(display,"-");
    if(S_IROTH&statbuf.st_mode)
        strcat(display,"r");
    else
        strcat(display,"-");
    if(S_IWOTH&statbuf.st_mode)
        strcat(display,"w");
    else
        strcat(display,"-");
    if(S_IXOTH&statbuf.st_mode)
        strcat(display,"x ");
    else
        strcat(display,"- ");
    char temp[10];
    strcat(display,getpwuid(statbuf.st_uid)->pw_name);
    strcat(display," ");
    strcat(display,getgrgid(statbuf.st_gid)->gr_name);
    strcat(display," ");
    sprintf(temp, "%8lu ", statbuf.st_size);
    strcat(display, temp);
    time_t t = statbuf.st_mtime;
    struct tm lt;
    localtime_r(&t, &lt);
    char timbuf[80];
    strftime(timbuf, sizeof(timbuf), "%c", &lt);
    strcat(display," ");
    strcat(display, timbuf);
    strcat(display,"\0");
    //printf("%s",display);
}

vector<struct file_and_folder> list_directory(char *path_name_for_directory){
    DIR *pointer_to_path_name_for_directory;
    struct dirent *entry;
    vector<struct file_and_folder> lst;
    if((pointer_to_path_name_for_directory = opendir(path_name_for_directory)) == NULL){
        fprintf(stderr, "cannot open directory: %s\n", path_name_for_directory);
        return lst;
    }
    while((entry=readdir(pointer_to_path_name_for_directory))!=NULL){
         if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
            continue;
        struct file_and_folder res;
        //res = (struct file_and_folder *)malloc(sizeof(struct file_and_folder));
        char *dsplay = (char *)malloc(100);
        memset(dsplay,0,sizeof(dsplay));
        file_or_directory_permission(path_name_for_directory,entry->d_name, dsplay);
        res.display = dsplay;
        res.path = entry->d_name;
        lst.push_back(res);
        //printf("%s \n",res.display);
        //printf(" %s\n",res.path);
    }
    /*for(int i=0;i<lst.size();i++)
        cout<<lst[i].display<<" "<<lst[i].path<<"\n";*/
    chdir("..");
    closedir(pointer_to_path_name_for_directory);
    return lst;
}