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

#include "commandmode/list_directory.h"
#include "commandmode/goto_directory.h"

#define cursorupward(x) printf("\033[%dA", (x))
#define cursordownward(x) printf("\033[%dB", (x))

#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108

static struct termios term, oterm;

static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);

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
    for(int i=0;i<lst.size();i++)
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
            //getcwd(root,100);
            //cout<<row<<endl;
            if(goto_directory(path_name,lst[row].path)==1){
                printf("\033[H\033[J");
                printf("\033[3J");
                lst.clear();
                strcat(path_name,"/");
                strcat(path_name,lst[row].path);
                string path = path_name;
                stack_prev.push(path);
                //cout<<stack_prev.top().path<<" "<<endl;
                lst = list_directory(path_name);
                for(int i=0;i<num_of_row && i<lst.size();i++)
                    cout<<lst[i].display<<" "<<lst[i].path<<"\n";
                //cout<<lst[num_of_row-1].display<<" "<<lst[num_of_row-1].path;
                //cout<<stack_prev.size()<<" "<<stack_next.size()<<endl;
                //cout<<stack_prev.top();//<<" "<<stack_next.top().path<<endl;
                gotoxy(0,0);
                row=0;
            }
        }
        else if (c == KEY_UP){
            /*if(index>0)*/
                row--;
            /*if(row==-1){
                index--;
            }*/
            cursorupward(1);
        } 
        else if(c == KEY_DOWN) {
            row++;
            /*if(row==num_of_row){
                index++;
            }*/
            cursordownward(1);
        } 
        else if (c == KEY_RIGHT) {
            //cout<<"hey"<<endl;
            //cout<<stack_next.top().path<<endl;
            if(!stack_next.empty()){
                printf("\033[H\033[J");
                printf("\033[3J");
                stack_prev.push(stack_next.top());
                stack_next.pop();
                //memset(path_name, 0 , strlen(path_name));
                strcpy(path_name, stack_prev.top().c_str());
                //cout<<stack_next.size()<<endl;
                //cout<<temp.path<<endl;
                lst.clear();
                lst = list_directory(path_name);
                for(int i=0;i<num_of_row && i<lst.size();i++)
                    cout<<lst[i].display<<" "<<lst[i].path<<"\n";
                //cout<<stack_prev.top()<<" "<<stack_next.top()<<endl;
                //cout<<stack_prev.size()<<" "<<stack_next.size()<<endl;
                gotoxy(0,0);
                row=0;
            }
        } 
        else if (c == KEY_LEFT) {
            //cout<<root<<endl;
            if(stack_prev.size()==1){
                    while(!stack_next.empty())
                        stack_next.pop();
            }
            else if(stack_prev.size()>1){
                //cout<<stack_prev.top().path<<endl;
                //cout<<root<<endl;
                printf("\033[H\033[J");
                printf("\033[3J");
                stack_next.push(stack_prev.top());
                stack_prev.pop();
                //memset(path_name, 0 , strlen(path_name));
                strcpy(path_name, stack_prev.top().c_str());
                //cout<<"HEY"<<endl;
                lst.clear();
                lst = list_directory(path_name);
                //cout<<path_name<<endl;
                for(int i=0;i<num_of_row && i<lst.size();i++)
                    cout<<lst[i].display<<" "<<lst[i].path<<"\n";
                //cout<<stack_next.top().path<<endl;
                //cout<<stack_next.size()<<endl;
                //cout<<stack_prev.top()<<" "<<stack_next.top()<<endl;
                //cout<<stack_prev.size()<<" "<<stack_next.size()<<endl;
                gotoxy(0,0);
                row=0;
            }
        }
        else if (c == KEY_ESCAPE){
            //row--;
        } 
        if(c==':'){
            cout<<"entered command mode";
        }
        else if(c == 'h' || c == 'H'){
            memset(path_name,0,sizeof(path_name));
            strcpy(path_name,root);
        }
        else if(c == 'q' || c == 'Q'){
                printf("\033[H\033[J");
                printf("\033[3J");
                gotoxy(0,0);
        }
    }
    printf("\n");
    return 0;
}