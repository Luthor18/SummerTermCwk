#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#define DELAY 30000

int main(int argc, char *argv[]) {
	int i = 0, c=0;
	int newX[8] = {0};
	int centipedex[8] = {0, 1, 2, 3, 4, 5, 6, 7 };
	int centipedey[8] = {0};
	int shipx, shipy;
	int projx, projy, projFired=0;
	int collision[8] = {0};
	int max_y = 0, max_x = 0;
	int direction[8] = {1, 1, 1, 1, 1, 1, 1, 1};
	char userInput;
	srand(time(NULL));  
	int r = rand();
	
	
	initscr();
	noecho();
	curs_set(FALSE);
	getmaxyx(stdscr, max_y, max_x);
	int mushrooms[max_y][max_x];
	
	shipx = max_x/2;
	shipy = max_y-1;
	nodelay(stdscr, TRUE); //stop the program from delaying while waiting for an input
 while(1) {
	getmaxyx(stdscr, max_y, max_x);
	clear();
	userInput = getch();
	for (i=0;i<8;i++){
		newX[i] = centipedex[i]+direction[i];
		if (newX[i]>=max_x || newX[i]<0){
			centipedey[i]+=1;
			direction[i]*= -1;
		}
		else{
			centipedex[i]+=direction[i];
		}
		if (collision[i] ==0 ){
			mvprintw(centipedey[i], centipedex[i], "o");
		}
	}
	if (userInput == 'a'){
		if (!((shipx-1)<=0)){
			shipx--;
		}
	}
	if (userInput == 'd'){
		if (!((shipx+1)>=max_x)){
			shipx++;
		}
	}
	if (projFired == 0){
		if (userInput == 'w'){	
		projFired = 1;
		projx=shipx;
		projy=shipy;
		}
	}
	
	if (projFired ==1){
		if (projy<=0){
			projy=shipy;
			projFired=0;
		}
		for (i=0;i<8;i++){
			if (projy==centipedey[i]&&projx==centipedex[i]){
				collision[i] = 1;
			}
		}
		projy--;
		mvprintw(projy, projx, "*");
	}
	shipy = max_y-1;
	mvprintw(shipy, shipx, "W");
	
	
	refresh();
	usleep(DELAY);
	userInput = 't';
	endwin();
	}
}
