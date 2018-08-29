#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <vector>
#include <iostream>
#include <stack>
#include <string>
using namespace std;

#include "commandmode/snapshot_directory.h"
#include "commandmode/copy_block.h"
#include "commandmode/create_file.h"
#include "commandmode/create_directory.h"
#include "commandmode/copy_files_inside_directory.h"
#include "commandmode/copy_directory.h" 
#include "commandmode/delete_directory.h" 
#include "commandmode/delete_file.h" 
#include "commandmode/goto_directory.h"
#include "commandmode/list_directory.h"
#include "commandmode/make_directory.h"
#include "commandmode/make_source_directory.h"
#include "commandmode/move_directory.h"
#include "commandmode/move_file.h"
#include "commandmode/rename_file.h"
#include "commandmode/search_file_or_directory.h"
#include "commandmode/snapshot_directory.h" 
#include "commandmode/trim_pathname.h"

#define cursorupward(x) printf("\033[%dA", (x))
#define cursordownward(x) printf("\033[%dB", (x))

#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108
#define BACKSPACE      127

static struct termios term, oterm;

static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);
void close_keyboard(){
    tcsetattr(0, TCSANOW, &oterm);
}

static int getch(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    return c;
}

static int kbhit(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

static int kbesc(void)
{
    int c;
    if (!kbhit()) return KEY_ESCAPE;
    c = getch();
    if (c == '[') {
        switch (getch()) {
            case 'A':
                c = KEY_UP;
                break;
            case 'B':
                c = KEY_DOWN;
                break;
            case 'D':
                c = KEY_LEFT;
                break;
            case 'C':
                c = KEY_RIGHT;
                break;
            default:
                c = 0;
                break;
        }
    } else {
        c = 0;
    }
    if (c == 0) while (kbhit()) getch();
    return c;
}

static int kbget(void)
{
    int c;

    c = getch();
    return (c == KEY_ESCAPE) ? kbesc() : c;
}

void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,x,y);
    fflush(stdout);
}

struct file_and_folder{
    char *display;
    char *path;
};
vector<string> split(string phrase, string delimiter){
    vector<string> list;
    string s = string(phrase);
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}
void commandmode(char *root){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int num_of_row = w.ws_row;
    printf("\033[H\033[J");
    printf("\033[3J");
    gotoxy(num_of_row,0);
    close_keyboard();
    int c;
    char cstrpath[1000];
    char cstrdest[1000];
    while(1){
        printf("\033[H\033[J");
        printf("\033[3J");
        gotoxy(num_of_row,0);
        string str;
        getline(cin, str);
        vector<string> command;
        if(str[0] != KEY_ESCAPE){
            command = split(str, " ");
            if(command[0] == "copy"){
                string complete_dest;
                string dest = command[command.size()-1];
                //cout<<dest[0]<<endl;;
                if(dest[0]=='~'){
                    complete_dest = root + dest.substr(1);
                    //cout<<complete_dest;
                }
                else
                    complete_dest = dest;
                for(int i=1;i<command.size()-1;i++){
                    if(command[i].find('.')!=string::npos){
                        string str1 = root;
                        string str2 = complete_dest;
                        str1.append("/");
                        str1 = str1 +  command[i];
                        strcpy(cstrpath, str1.c_str());
                        strcpy(cstrdest, str2.c_str());
                        //cout<<complete_path<<endl;
                        copy_block(cstrpath,cstrdest);
                    }
                    else{
                        //cout<<command[i]<<endl;
                        string str1 = root;
                        string str2 = complete_dest;
                        str1.append("/");
                        str1 = str1 +  command[i];
                        strcpy(cstrpath, str1.c_str());
                        strcpy(cstrdest, str2.c_str());
                        //cout<<complete_path<<endl;
                        copy_directory(cstrpath,cstrdest);
                    }
                }
            }
            else if(command[0] == "move"){
                string complete_dest;
                string dest = command[command.size()-1];
                //cout<<dest[0]<<endl;;
                if(dest[0]=='~'){
                    complete_dest = root + dest.substr(1);
                    //cout<<complete_dest;
                }
                else
                    complete_dest = dest;
                for(int i=1;i<command.size()-1;i++){
                    if(command[i].find('.')!=string::npos){
                        string str1 = root;
                        string str2 = complete_dest;
                        str1.append("/");
                        str1 = str1 +  command[i];
                        strcpy(cstrpath, str1.c_str());
                        strcpy(cstrdest, str2.c_str());
                        //cout<<complete_path<<endl;
                        move_file(cstrpath,cstrdest);
                    }
                    else{
                        cout<<command[i]<<endl;
                        string str1 = root;
                        string str2 = complete_dest;
                        str1.append("/");
                        str1 = str1 +  command[i];
                        strcpy(cstrpath, str1.c_str());
                        strcpy(cstrdest, str2.c_str());
                        //cout<<complete_path<<endl;
                        move_directory(cstrpath,cstrdest);
                    }
                }
            }
            else if(command[0] == "rename"){
                string str1 = root;
                str1.append("/");
                string str2 = str1 + command[2]; 
                str1 = str1 +  command[1];
                strcpy(cstrpath, str1.c_str());
                strcpy(cstrdest, str2.c_str());
                rename_file(cstrpath,cstrdest);
            }
            else if(command[0] == "create_file"){
                string complete_dest;
                string dest = command[command.size()-1];
                //cout<<dest[0]<<endl;;
                if(dest[0]=='~')
                    complete_dest = root + dest.substr(1);
                    //cout<<complete_dest;
                else if(dest==".")
                    complete_dest = root;
                else
                    complete_dest = dest;
                cout<<complete_dest<<endl;
                for(int i=1;i<command.size()-1;i++){
                        string str1 = command[i];
                        strcpy(cstrpath, str1.c_str());
                        strcpy(cstrdest, complete_dest.c_str());
                        create_file(cstrpath, cstrdest);
                        //cout<<cstrpath<<" "<<cstrdest<<endl;
                }
            }
            else if(command[0] == "create_dir"){
                string complete_dest;
                string dest = command[command.size()-1];
                //cout<<dest[0]<<endl;;
                if(dest[0]=='~')
                    complete_dest = root + dest.substr(1);
                    //cout<<complete_dest;
                else if(dest==".")
                    complete_dest = root;
                else
                    complete_dest = dest;
                for(int i=1;i<command.size()-1;i++){
                        string str1 = command[i];
                        strcpy(cstrpath, str1.c_str());
                        strcpy(cstrdest, complete_dest.c_str());
                        //cout<<complete_dest<<endl;
                        create_directory(cstrpath, cstrdest);
                        //cout<<cstrpath<<" "<<cstrdest<<endl;
                }
            }
            else if(command[0] == "delete_file"){
                for(int i=1;i<command.size();i++){
                    string str1 = root;
                    if(command[i][0]=='~')
                        str1 = str1 + command[i].substr(1);
                    else{
                        str1.append("/");
                        str1 = str1 + command[i];
                    }
                    strcpy(cstrpath, str1.c_str());
                    //cout<<complete_dest<<endl;
                    delete_file(cstrpath);
                    //cout<<cstrpath;//<<" "<<cstrdest<<endl;
                }
            }
            else if(command[0] == "delete_dir"){
                for(int i=1;i<command.size();i++){
                    string str1 = root;
                    if(command[i][0]=='~')
                        str1 = str1 + command[i].substr(1);
                    else{
                        str1.append("/");
                        str1 = str1 + command[i];
                    }
                    strcpy(cstrpath, str1.c_str());
                    //cout<<complete_dest<<endl;
                    delete_directory(cstrpath);
                    //cout<<cstrpath;//<<" "<<cstrdest<<endl;
                }
            }
            else if(command[0] == "goto"){
                
            }
            else if(command[0] == "search"){
                string str1 = root;
                string str2 = command[1];
                strcpy(cstrpath, str1.c_str());
                strcpy(cstrdest, str2.c_str());
                printf("\033[H\033[J");
                printf("\033[3J");
                search_file_or_directory(cstrpath,cstrdest,cstrpath);
                gotoxy(0,0);
                sleep(10);
            }
            else if(command[0] == "snapshot"){
                string str1 = root;
                string str2;
                str1.append("/");
                str2 = str1;
                str1 = str1 +  command[1];
                str2 = str2 + command[2];
                strcpy(cstrpath, str1.c_str());
                strcpy(cstrdest, str2.c_str());
                freopen(cstrdest,"w", stdout);
                snapshot_directory(cstrpath,cstrpath);
                fclose(stdout);
                fflush(stdin);
                fflush(stdout);
                freopen ("/dev/tty", "a", stdout);
                fflush(stdout);
                cout.clear();
                printf("\033[H\033[J");
                printf("\033[3J");
                snapshot_directory(cstrpath,cstrpath);
                gotoxy(0,0);
                sleep(10);
                //stdout = fdopen(1, "w");
            }
        }
        else
            break;
    }
}

int main(void)
{
    int c;
    int ch=0;
    printf("\033[H\033[J");
    printf("\033[3J");
    char root[500];
    getcwd(root,100);
    vector<struct file_and_folder> lst;
    stack<string> stack_prev;
    stack<string> stack_next;
    lst = list_directory(root);
    int row=0;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int num_of_row = w.ws_row;
    for(int i=0;i<num_of_row-1 && i<lst.size();i++)
        cout<<lst[i].display<<" "<<lst[i].path<<"\n";
    stack_prev.push(root);
    //cout<<stack_prev.top().path<<" "<<endl;
    //cout<<lst[num_of_row-1].display<<" "<<lst[num_of_row-1].path;
    gotoxy(0,0);
    char path_name[500];
    int index=0;
    strcpy(path_name, root);
     while (c!='q') {
        c = kbget();
        if (c == KEY_ENTER){
            if(row==1 && strcmp(path_name,root)){
                int i=0;
                lst.clear();
                for(i=strlen(path_name)-1;i>=0;i--){
                    if(path_name[i]=='/')
                        break;
                }
                path_name[i]='\0';
                string path = path_name;
                stack_prev.push(path);
                while(!stack_next.empty())
                    stack_next.pop();
                lst = list_directory(path_name);
                //sort(lst.begin(),lst.end(),compare_names);
                printf("\033[H\033[J");
                printf("\033[3J");
                for(int i=0;i<num_of_row-1 && i<lst.size();i++)
                    cout<<lst[i].display<<" "<<lst[i].path<<"\n";
                gotoxy(0,0);
                row=0;
            }
            else if(goto_directory(path_name,lst[row].path)==1){
                printf("\033[H\033[J");
                printf("\033[3J");
                lst.clear();
                strcat(path_name,"/");
                strcat(path_name,lst[row].path);
                string path = path_name;
                stack_prev.push(path);
                while(!stack_next.empty())
                    stack_next.pop();
                lst = list_directory(path_name);
                //sort(lst.begin(),lst.end(),compare_names);
                for(int i=0;i<num_of_row-1 && i<lst.size();i++)
                    cout<<lst[i].display<<" "<<lst[i].path<<"\n";
                gotoxy(0,0);
                row=0;
            }
        }
        else if (c == KEY_UP){
            if(row>0)
                row--;
            /*if(row==-1){
                index--;
            }*/
            cursorupward(1);
        } 
        else if(c == KEY_DOWN) {
            if(row<num_of_row-1)
                row++;
            /*if(row==num_of_row-){
                index++;
            }*/
            cursordownward(1);
        } 
        else if (c == KEY_RIGHT) {
            if(!stack_next.empty()){
                printf("\033[H\033[J");
                printf("\033[3J");
                stack_prev.push(stack_next.top());
                stack_next.pop();
                strcpy(path_name, stack_prev.top().c_str());
                lst.clear();
                lst = list_directory(path_name);
                //sort(lst.begin(),lst.end(),compare_names);
                for(int i=0;i<num_of_row-1 && i<lst.size();i++)
                    cout<<lst[i].display<<" "<<lst[i].path<<"\n";
                gotoxy(0,0);
                row=0;
            }
        } 
        else if (c == KEY_LEFT) {
            if(stack_prev.size()>1){
                printf("\033[H\033[J");
                printf("\033[3J");
                stack_next.push(stack_prev.top());
                stack_prev.pop();
                strcpy(path_name, stack_prev.top().c_str());
                lst.clear();
                lst = list_directory(path_name);
                //sort(lst.begin(),lst.end(),compare_names);
                for(int i=0;i<num_of_row-1 && i<lst.size();i++)
                    cout<<lst[i].display<<" "<<lst[i].path<<"\n";
                gotoxy(0,0);
                row=0;
            }
        }
        else if(c == BACKSPACE){
            if(strcmp(root,path_name)){
                printf("\033[H\033[J");
                printf("\033[3J");
                lst.clear();
                int i=0;
                while(!stack_prev.empty())
                    stack_prev.pop();
                while(!stack_next.empty()){
                    stack_next.pop();
                }
                string path = path_name;
                stack_prev.push(path);
                for(i=strlen(path_name)-1;i>=0;i--){
                    if(path_name[i]=='/')
                        break;
                }
                path_name[i]='\0';
                path = path_name;
                stack_prev.push(path);
                lst = list_directory(path_name);
                //sort(lst.begin(),lst.end(),compare_names);
                for(int i=0;i<num_of_row-1 && i<lst.size();i++)
                    cout<<lst[i].display<<" "<<lst[i].path<<"\n";
                gotoxy(0,0);
                row=0;
            }
        }
        else if (c == KEY_ESCAPE){
            //row--;
        } 
        if(c==':'){
            cout<<"entered command mode";
            commandmode(path_name);
            cout<<"\033[H\033[J";
            cout<<"\033[3J";
            fflush(stdout);
            cout.clear();
            lst = list_directory(path_name);
            for(int i=0;i<num_of_row-1 && i<lst.size();i++)
                    cout<<lst[i].display<<" "<<lst[i].path<<"\n";

            gotoxy(0,0);
            row = 0;
        }
        else if(c == 'h' || c == 'H'){
            memset(path_name,0,sizeof(path_name));
            strcpy(path_name,root);
            string path = path_name;
            stack_prev.push(path);
            lst = list_directory(path_name);
            //sort(lst.begin(),lst.end(),compare_names);
            for(int i=0;i<num_of_row-1 && i<lst.size();i++)
                cout<<lst[i].display<<" "<<lst[i].path<<"\n";
            gotoxy(0,0);
            row=0;
        }
        else if(c == 'q' || c == 'Q'){
                printf("\033[H\033[J");
                printf("\033[3J");
                gotoxy(0,0);
        }
    }
    return 0;
}