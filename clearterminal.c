#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("\033[H\033[J");
    printf("\033[3J");
    exit(0);
}