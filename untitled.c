#include <ncurses.h>
#include <unistd.h>

#define DELAY 30000

int main(int argc, char *argv[]) {
	int i = 0, c=0;
	int newX[8] = {0};
	int centipedex[8] = {0, 1, 2, 3, 4, 5, 6, 7 };
	int centipedey[8] = {0};
	int max_y = 0, max_x = 0;
	
	int direction[8] = {1, 1, 1, 1, 1, 1, 1, 1};

	initscr();
	noecho();
	curs_set(FALSE);
 while(1) {
	getmaxyx(stdscr, max_y, max_x);
	clear();
	
	for (i=0;i<8;i++){
		newX[i] = centipedex[i]+direction[i];
		if (newX[i]>=max_x || newX[i]<0){
			centipedey[i]+=1;
			direction[i]*= -1;
		}
		else{
			centipedex[i]+=direction[i];
		}
		mvprintw(centipedey[i], centipedex[i], "o");
	}
	refresh();
	usleep(DELAY);
endwin();
}
}
