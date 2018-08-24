#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("\033[H\033[J");
    printf("\033[3J");
    printf("\033[0;0H");
    printf("\033[H\033[2J");
    int n;
    scanf("%d",&n);
    exit(0);
}