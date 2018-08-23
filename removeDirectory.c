#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int remove_directory(char *path){
    DIR *d;
    size_t path_len = strlen(path);
    int r = -1;
    if((d = opendir(path)) == NULL){
        fprintf(stderr, "cannot open directory: %s\n", path);
        return 0;
    }
    chdir(path);
    if(d){
        struct dirent *p;
        r=0;
        while(!r && (p=readdir(d))){
            int r2 = -1;
            char *buffer;
            size_t len;
            if(!strcmp(p->d_name,".") || !strcmp(p->d_name,".."))
                continue;
            len = path_len + strlen(p->d_name)+2;
            buffer = (char *)malloc(len);

            if(buffer){
                struct stat statbuffer;
                snprintf(buffer, len, "%s/%s", path, p->d_name);
                if(!stat(buffer, &statbuffer)){
                    if(S_ISDIR(statbuffer.st_mode))
                        r2 = remove_directory(buffer);
                    else
                        r2 = unlink(buffer);
                }
                free(buffer);
            }
            r = r2;
        }
        chdir("..");
        closedir(d);
        if(!r)
            r = rmdir(path);
        return r;
    }
}

int main(int argc, char* argv[]){
    char *topdir = ".";
    if(argc>=2)
        topdir = argv[1];
    printf("Remove Directory of %s:\n", topdir);
    remove_directory(topdir);
    printf("done.\n");
    exit(0);
}