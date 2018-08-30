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

static struct termios newsettings, oldsettings;

static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);
void close_keyboard(){
    tcsetattr(0, TCSANOW, &oldsettings);
}

static int getch(void)
{
    int c = 0;

    tcgetattr(0, &oldsettings);
    memcpy(&newsettings, &oldsettings, sizeof(newsettings));
    newsettings.c_lflag &= ~(ICANON | ECHO);
    newsettings.c_cc[VMIN] = 1;
    newsettings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &newsettings);
    c = getchar();
    tcsetattr(0, TCSANOW, &oldsettings);
    return c;
}

static int kbhit(void)
{
    int c = 0;

    tcgetattr(0, &oldsettings);
    memcpy(&newsettings, &oldsettings, sizeof(newsettings));
    newsettings.c_lflag &= ~(ICANON | ECHO);
    newsettings.c_cc[VMIN] = 0;
    newsettings.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &newsettings);
    c = getchar();
    tcsetattr(0, TCSANOW, &oldsettings);
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
string commandmode(vector<struct file_and_folder> lst, char *root, char *root_dir, int start, int end){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int num_of_row = w.ws_row;
    int num_of_col = w.ws_col;
    printf("\033[H\033[J");
    printf("\033[3J");
    gotoxy(0,num_of_col/2-5);
    printf("\u001b[0m\u001b[7m COMMAND MODE \u001b[0m\n");
    for(int i=start;i<end && i<lst.size();i++){
        cout<<lst[i].display<<" ";
        if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
            cout<<lst[i].path<<"\n";
        else{
            for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                cout<<lst[i].path[j];
            cout<<"\n";
        }
    } 
    gotoxy(num_of_row,0);
    //close_keyboard();
    int c;
    char cstrpath[1000];
    char cstrdest[1000];
    while(1){
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        num_of_row = w.ws_row;
        num_of_col = w.ws_col;
        printf("\033[H\033[J");
        printf("\033[3J");
        gotoxy(0,num_of_col/2-5);
        printf("\u001b[0m\u001b[7m COMMAND MODE \u001b[0m\n");
        for(int i=start;i<end && i<lst.size();i++){
            cout<<lst[i].display<<" ";
            if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
                cout<<lst[i].path<<"\n";
            else{
                for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                    cout<<lst[i].path[j];
                cout<<"\n";
            }
        } 
        gotoxy(num_of_row,0);
        string str;
        while((c=kbget())!='\n' && c!=KEY_ESCAPE){
            str = str + (char)c;
            putchar(c);
        }
        if(c==KEY_ESCAPE)
            break;
        //getline(cin, str);
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
                        if(command[i][0]!='/')
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
                        if(command[i][0]!='/')
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
                        if(command[i][0]!='/')
                            str1.append("/");
                        str1 = str1 +  command[i];
                        strcpy(cstrpath, str1.c_str());
                        strcpy(cstrdest, str2.c_str());
                        //cout<<complete_path<<endl;
                        move_file(cstrpath,cstrdest);
                    }
                    else{
                        //cout<<command[i]<<endl;
                        string str1 = root;
                        string str2 = complete_dest;
                        if(command[i][0]!='/')
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
                if(command[1][0]!='/')
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
                //cout<<complete_dest<<endl;
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
                        if(command[i][0]!='/')
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
                        if(command[i][0]!='/')
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
                if(command[1]=="/"){
                    return root_dir;
                }
                string str1 = root_dir;
                if(command[1][0]!='/')
                    str1.append("/");
                str1 = str1 + command[1];
                strcpy(cstrpath, command[1].c_str());
                return str1;
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
                sleep(5);
            }
            else if(command[0] == "snapshot"){
                string str1 = root;
                string str2;
                if(command[1][0]!='/')
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
                sleep(5);
                //stdout = fdopen(1, "w");
            }
        }
        else
            break;
    }
    return root;
}

int main(void)
{
    int c;
    printf("\033[H\033[J");
    printf("\033[3J");
    char root[500];
    getcwd(root,100);
    vector<struct file_and_folder> lst;
    stack<string> stack_prev;
    stack<string> stack_next;
    lst = list_directory(root);
    int row=2;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int num_of_row = w.ws_row;
    int num_of_col = w.ws_col;
    gotoxy(0,num_of_col/2 -5);
    printf("\u001b[0m\u001b[7m NORMAL MODE \u001b[0m\n");
    int start,end;
    start = 0;
    end = num_of_row-2;
    for(int i=start;i<end && i<lst.size();i++){
        cout<<lst[i].display<<" ";
        if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
            cout<<lst[i].path<<"\n";
        else{
            for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                cout<<lst[i].path[j];
            cout<<"\n";
        }
    }
    stack_prev.push(root);
    //cout<<stack_prev.top().path<<" "<<endl;
    //cout<<lst[num_of_row-1].display<<" "<<lst[num_of_row-1].path;
    gotoxy(2,0);
    char path_name[500];
    strcpy(path_name, root);
     while (c!='q') {
        c = kbget();
        if (c == KEY_ENTER){
            if(row==3 && strcmp(path_name,root)){
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
                int num_of_row = w.ws_row;
                int num_of_col = w.ws_col;
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
                gotoxy(0,num_of_col/2 -5);
                printf("\u001b[0m\u001b[7m NORMAL MODE \u001b[0m\n");
                for(int i=start;i<end && i<lst.size();i++){
                    cout<<lst[i].display<<" ";
                    if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
                        cout<<lst[i].path<<"\n";
                    else{
                        for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                            cout<<lst[i].path[j];
                        cout<<"\n";
                    }
                }
                gotoxy(2,0);
                row=2;
            }
            else if(goto_directory(path_name,lst[start+row-2].path)==1){
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
                int num_of_row = w.ws_row;
                int num_of_col = w.ws_col;
                printf("\033[H\033[J");
                printf("\033[3J");
                gotoxy(0,num_of_col/2 -5);
                printf("\u001b[0m\u001b[7m NORMAL MODE \u001b[0m\n");
                lst.clear();
                strcat(path_name,"/");
                strcat(path_name,lst[start+row-2].path);
                string path = path_name;
                stack_prev.push(path);
                while(!stack_next.empty())
                    stack_next.pop();
                lst = list_directory(path_name);
                //sort(lst.begin(),lst.end(),compare_names);
                for(int i=start;i<end && i<lst.size();i++){
                    cout<<lst[i].display<<" ";
                    if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
                        cout<<lst[i].path<<"\n";
                    else{
                    for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                        cout<<lst[i].path[j];
                    cout<<"\n";
                    }
                }           
                gotoxy(2,0);
                row=2;
                start=0;
                end = num_of_row-2;
            }
        }
        else if (c == KEY_UP){
            if(start>0 && row==num_of_row-1){   
                start--;
                end--;
                printf("\033[H\033[J");
                printf("\033[3J");
                gotoxy(0,num_of_col/2 -5);
                printf("\u001b[0m\u001b[7m NORMAL MODE \u001b[0m\n");
                for(int i=start;i<end && i<lst.size();i++){
                    cout<<lst[i].display<<" ";
                    if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
                        cout<<lst[i].path<<"\n";
                    else{
                        for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                            cout<<lst[i].path[j];
                        cout<<"\n";
                    }
                }
                gotoxy(row,0);
            }
            else if(row>2){
                row--;
                cursorupward(1);
            }
        } 
        else if(c == KEY_DOWN) {
            if(row==num_of_row-1 && start>=0 && end<lst.size()){
                start++;
                end++;
                printf("\033[H\033[J");
                printf("\033[3J");
                gotoxy(0,num_of_col/2 -5);
                printf("\u001b[0m\u001b[7m NORMAL MODE \u001b[0m\n");
                for(int i=start;i<end && i<lst.size();i++){
                    cout<<lst[i].display<<" ";
                    if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
                        cout<<lst[i].path<<"\n";
                    else{
                        for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                            cout<<lst[i].path[j];
                        cout<<"\n";
                    }
                }
                gotoxy(row,0);
            }
            else if(row<num_of_row-1 && row<=lst.size()){
                row++;    
                cursordownward(1);
            }
        } 
        else if (c == KEY_RIGHT) {
            if(!stack_next.empty()){
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
                int num_of_row = w.ws_row;
                int num_of_col = w.ws_col;
                printf("\033[H\033[J");
                printf("\033[3J");
                gotoxy(0,num_of_col/2 -5);
                printf("\u001b[0m\u001b[7m NORMAL MODE \u001b[0m\n");
                stack_prev.push(stack_next.top());
                stack_next.pop();
                strcpy(path_name, stack_prev.top().c_str());
                lst.clear();
                lst = list_directory(path_name);
                //sort(lst.begin(),lst.end(),compare_names);
                for(int i=start;i<end && i<lst.size();i++){
                    cout<<lst[i].display<<" ";
                    if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
                        cout<<lst[i].path<<"\n";
                    else{
                        for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                            cout<<lst[i].path[j];
                        cout<<"\n";
                    }
                }
                gotoxy(2,0);
                row=2;
            }
        } 
        else if (c == KEY_LEFT) {
            if(stack_prev.size()>1){
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
                int num_of_row = w.ws_row;
                int num_of_col = w.ws_col;
                printf("\033[H\033[J");
                printf("\033[3J");
                gotoxy(0,num_of_col/2 -5);
                printf("\u001b[0m\u001b[7m NORMAL MODE \u001b[0m\n");
                stack_next.push(stack_prev.top());
                stack_prev.pop();
                strcpy(path_name, stack_prev.top().c_str());
                lst.clear();
                lst = list_directory(path_name);
                //sort(lst.begin(),lst.end(),compare_names);
                for(int i=start;i<end && i<lst.size();i++){
                    cout<<lst[i].display<<" ";
                    if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
                        cout<<lst[i].path<<"\n";
                    else{
                        for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                            cout<<lst[i].path[j];
                        cout<<"\n";
                    }
                }
                gotoxy(2,0);
                row=2;
            }
        }
        else if(c == BACKSPACE){
            if(strcmp(root,path_name)){
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
                int num_of_row = w.ws_row;
                int num_of_col = w.ws_col;
                printf("\033[H\033[J");
                printf("\033[3J");
                gotoxy(0,num_of_col/2 -5);
                printf("\u001b[0m\u001b[7m NORMAL MODE \u001b[0m\n");
                lst.clear();
                int i=0;
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
                for(int i=start;i<end && i<lst.size();i++){
                    cout<<lst[i].display<<" ";
                    if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
                        cout<<lst[i].path<<"\n";
                    else{
                        for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                            cout<<lst[i].path[j];
                        cout<<"\n";
                    }
                }
                gotoxy(2,0);
                row=2;
            }
        }
        else if (c == KEY_ESCAPE){
            //row--;
        } 
        if(c==':'){
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            int num_of_row = w.ws_row;
            int num_of_col = w.ws_col;
            //cout<<"entered command mode";
            string str1 = commandmode(lst,path_name, root, start, end);
            cout<<"\033[H\033[J";
            cout<<"\033[3J";
            gotoxy(0,num_of_col/2 -5);
            printf("\u001b[0m\u001b[7m NORMAL MODE \u001b[0m\n");
            fflush(stdout);
            cout.clear();
            while(!stack_next.empty())
                stack_next.pop();
            stack_prev.push(str1);
            strcpy(path_name,str1.c_str());
            lst = list_directory(path_name);
            for(int i=start;i<end && i<lst.size();i++){
                cout<<lst[i].display<<" ";
                if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
                    cout<<lst[i].path<<"\n";
                else{
                    for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                        cout<<lst[i].path[j];
                    cout<<"\n";
                }
            }
            gotoxy(2,0);
            row = 2;
        }
        else if(c == 'h' || c == 'H'){
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            int num_of_row = w.ws_row;
            int num_of_col = w.ws_col;
            printf("\033[H\033[J");
            printf("\033[3J");
            gotoxy(0,num_of_col/2 -5);
            printf("\u001b[0m\u001b[7m NORMAL MODE \u001b[0m\n");
            memset(path_name,0,sizeof(path_name));
            strcpy(path_name,root);
            string path = path_name;
            while(!stack_next.empty())
                stack_next.pop();
            stack_prev.push(path);
            lst = list_directory(path_name);
            //sort(lst.begin(),lst.end(),compare_names);
            for(int i=start;i<end && i<lst.size();i++){
                cout<<lst[i].display<<" ";
                if(strlen(lst[i].display)+strlen(lst[i].path)<num_of_col)
                    cout<<lst[i].path<<"\n";
                else{
                    for(int j=0;(strlen(lst[i].display) + j + 1 < num_of_col);j++)
                        cout<<lst[i].path[j];
                    cout<<"\n";
                }
            }
            gotoxy(2,0);
            row=2;
            start=0;
            end = num_of_row-2;
        }
        else if(c == 'q' || c == 'Q'){
                printf("\033[H\033[J");
                printf("\033[3J");
                gotoxy(0,0);
        }
    }
    return 0;
}