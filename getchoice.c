#include <stdio.h>
#include <stdlib.h>

char *menu[]={
    "a - add new record",
    "d - delete record",
    "q - quit",
    NULL,
};

int getchoice(char *greet, char *choice[]){
    int chosen = 0;
    int selected;
    char **option;

    do{
        printf("Choice: %s\n", greet);
        option = choice;
        while(*option){
            printf("%s\n", *option);
            option++;
        }
        selected = getchar();
        option = choice;
        
        while(*option){
            if(selected == *option[0]){
                chosen = 1;
                break;
            }
            option++;
            do{
            selected = getchar();
            }while(selected=='\n');
        }
        if(!chosen){
            printf("Incorrect choice, select again\n");
        }
    }while(!chosen);
    return selected;
}

int main(){
    int choice = 0;
    do{
        choice = getchoice("Please select an action", menu);
        printf("You have chosen %c\n", choice);
    }while(choice!='q');
    exit(0);
}