#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

#include "trim_pathname.h"

void trim_pathname(char *source_pathname, char *destination_pathname){
    size_t source_length = strlen(source_pathname);
    size_t destination_length = strlen(destination_pathname);
    int index_for_source_pathname=0;
    int index_for_destination_pathname=destination_length;
    for(index_for_source_pathname=source_length-1;index_for_source_pathname>=0;index_for_source_pathname--){
        if(*(source_pathname+index_for_source_pathname)=='/')
            break;
    }
    for(;index_for_source_pathname<source_length;index_for_source_pathname++,index_for_destination_pathname++)
        *(destination_pathname+index_for_destination_pathname)=*(source_pathname+index_for_source_pathname);
    *(destination_pathname+index_for_destination_pathname)='\0';
}