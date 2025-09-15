#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define W 20
#define H 20

int gameOver;
int hx, hy;       // head x, y
int fx, fy;       // fruit x, y
int tx[200], ty[200]; // tail
int tlen;         // tail length
int sc;           // score
int dir;          // 0 stop,1 left,2 right,3 up,4 down

void setup() {
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);
    timeout(100);

    gameOver = 0;
    dir = 0;
    hx = W/2;
    hy = H/2;
    fx = rand()%W;
    fy = rand()%H;
    sc = 0;
    tlen = 0;
}

void draw() {
    clear();
    for(int i=0;i<W+2;i++) mvprintw(0,i,"#");
    for(int i=0;i<H;i++){
        for(int j=0;j<W;j++){
            if(j==0) mvprintw(i+1,0,"#");

            if(i==hy && j==hx) mvprintw(i+1,j+1,"O");
            else if(i==fy && j==fx) mvprintw(i+1,j+1,"F");
            else {
                int pr=0;
                for(int k=0;k<tlen;k++){
                    if(tx[k]==j && ty[k]==i){
                        mvprintw(i+1,j+1,"o");
                        pr=1;
                    }
                }
                if(!pr) mvprintw(i+1,j+1," ");
            }

            if(j==W-1) mvprintw(i+1,j+1,"#");
        }
    }
    for(int i=0;i<W+2;i++) mvprintw(H+1,i,"#");
    mvprintw(H+3,0,"SCORE=%d",sc);
    refresh();
}

void input() {
    int c=getch();
    if(c=='a') dir=1;
    else if(c=='d') dir=2;
    else if(c=='w') dir=3;
    else if(c=='s') dir=4;
    else if(c=='x') gameOver=1;
}

void logic() {
    int px=tx[0],py=ty[0],px2,py2;
    tx[0]=hx; ty[0]=hy;
    for(int i=1;i<tlen;i++){
        px2=tx[i]; py2=ty[i];
        tx[i]=px; ty[i]=py;
        px=px2; py=py2;
    }

    if(dir==1) hx--;
    else if(dir==2) hx++;
    else if(dir==3) hy--;
    else if(dir==4) hy++;

    // wrap walls
    if(hx>=W) hx=0; else if(hx<0) hx=W-1;
    if(hy>=H) hy=0; else if(hy<0) hy=H-1;

    for(int i=0;i<tlen;i++){
        if(tx[i]==hx && ty[i]==hy) gameOver=1;
    }

    if(hx==fx && hy==fy){
        sc+=10;
        fx=rand()%W;
        fy=rand()%H;
        tlen++;
    }
}

int main() {
    setup();
    while(!gameOver){
        draw();
        input();
        logic();
        usleep(100000);
    }
    endwin();
    printf("GAME OVER : score=%d\n",sc);
    return 0;
}
