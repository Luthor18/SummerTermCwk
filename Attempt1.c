#include <ncurses.h>
#include <unistd.h>

#define DELAY 30000

int main(int argc, char *argv[]) {
	int i, c;
	int max_y = 0, max_x = 0;
	int left = 0;
	int centipede1[7][1];
	initscr();
	noecho();
	curs_set(FALSE);
 

 while(1) {
	getmaxyx(stdscr, max_y, max_x);
	clear();
	for (i=0;i<8-left;i++){
		centipede1[i][0]++;
	}
	for (c=7;c<8-left;c--){
		centipede1[c][0]--;
	}
	if(centipede1[7-left][0]>=max_x){
		centipede1[7-left][0]--;
		centipede1[7-left][1]++;
		left++;
	}
	if(centipede1[7-left][0]<=0){
		centipede1[7-left][0]++;
		centipede1[7-left][1]++;
		left--;
	}
	for(i=0;i<8;i++){
		mvprintw(centipede1[i][0], centipede1[i][1], "o");
	}
	refresh();

	usleep(DELAY);

}
endwin();
}
