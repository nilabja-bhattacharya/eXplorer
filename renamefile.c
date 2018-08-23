#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
int main(){
    char old_name[100];
    char new_name[100];
    int status;
    scanf("%s %s",old_name, new_name);
    status = rename(old_name,new_name);
    status==0?printf("%s File name changed successfully", old_name):printf("Error");
    exit(0);
}