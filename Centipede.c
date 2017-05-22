#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#define DELAY 30000

int main(int argc, char *argv[]) {
	int i = 0, c=0, t=0, cont = 1, endcounter = 0, timer=0, score=0, gamestate = 1;
	int randomnumber;
	srand(time(NULL));
	
	int newX[8] = {0};
	int centipedex[8] = {0, 1, 2, 3, 4, 5, 6, 7 };
	int centipedey[8] = {1, 1, 1, 1, 1, 1, 1, 1};
	int direction[8] = {1, 1, 1, 1, 1, 1, 1, 1};
	int collision[8] = {0};
	
	int shipx, shipy;
	
	int projx, projy, projFired=0;
	
	int max_y = 0, max_x = 0;
	
	char userInput;
	
	
	
	initscr();
	noecho();
	curs_set(FALSE);
	getmaxyx(stdscr, max_y, max_x);
	
	
	int mushrooms[max_y][max_x];
	for (i=0;i<max_y;i++){
		for(c=0;c<max_x;c++){
			mushrooms[i][c] = 0;
		}
	}
	for (i=2;i<max_y-1;i+=3){
		randomnumber = rand() % (max_x-6);
		mushrooms[i][3+randomnumber] = 2;
		randomnumber = rand() % (max_x-6);
		mushrooms[i][3+randomnumber] = 2;
	}
		
	shipx = max_x/2;
	shipy = max_y-1;
	
	nodelay(stdscr, TRUE); //stop the program from delaying while waiting for an input
	
	while(cont == 1){	
		while(gamestate==1) {
			timer++;
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
					if (mushrooms[centipedey[i]][newX[i]]!=0){
						centipedey[i]+=1;
						direction[i]*= -1;
					}
					else{
						centipedex[i]+=direction[i];
					}
					
				}
				
				if (collision[i] ==0 ){
					mvprintw(centipedey[i], centipedex[i], "o");
				}
				if (centipedex[i] == shipx){
					if (centipedey[i] == shipy){
						gamestate=0;
					}
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
						if (collision[i]==0){
							collision[i] = 1;
							score+=10;
							endcounter++;
						}
						
					}
				}
				if (mushrooms[projy][projx] == 2){
					mushrooms[projy][projx] = 1;
					projFired = 0;
					score++;
				}
				else if (mushrooms[projy][projx] == 1){
					mushrooms[projy][projx] = 0;
					projFired = 0;
					score++;
				}
				
				projy--;
				mvprintw(projy, projx, "*");
			}
			
			for (i=0;i<max_y;i++){
				for (c=0;c<max_x;c++){
					if (mushrooms[i][c] == 2){
						mvprintw(i, c, "X");
					}
					if (mushrooms[i][c] == 1){
						mvprintw(i, c, "x");
					}
				}
			}
			
			shipy = max_y-1;
			mvprintw(shipy, shipx, "W");
			mvprintw(0,0,"Timer: %d", timer/30);
			mvprintw(0, 10, "Score: %d", score);
			if (endcounter==8){
				gamestate = 2;
			}	
			refresh();
			usleep(DELAY);
			userInput = 't';
			endwin();
		}
		if (gamestate == 0){
			getmaxyx(stdscr, max_y, max_x);
			clear();
			mvprintw(max_y/2, (max_x/2)-5, "GAME OVER! YOU LOSE!");
			mvprintw((max_y/2)+1, (max_x/2)-5, "Score: %d    You survived for %d seconds.", score, timer/30);
			mvprintw((max_y/2)+2, (max_x/2)-5, "Play again? Yes or no?(y or n)");
			refresh();
			userInput = getch();
			usleep(1000);
			if (userInput=='y'){
				gamestate = 1;
				for (i=0;i<8;i++){
					centipedex[i] = i;
					centipedey[i] = 1;
					direction[i] = 1;
					collision[i] = 0;
					newX[i] = 0;
					endcounter = 0; 
					timer=0;
					score=0;
				}
			}
			if (userInput=='n'){
				cont = 0;
			}
		}
		
		if (gamestate == 2){
			getmaxyx(stdscr, max_y, max_x);
			clear();
			mvprintw(max_y/2, (max_x/2)-5, "CONGRATULATIONS! YOU WIN!");
			mvprintw((max_y/2)+1, (max_x/2)-5, "Score: %d    You survived for %d seconds.", score, timer/30);
			mvprintw((max_y/2)+2, (max_x/2)-5, "Play again? Yes or no?(y or n)");
			refresh();
			userInput = getch();
			usleep(1000);
			if (userInput=='y'){
				gamestate = 1;
				for (i=0;i<8;i++){
					centipedex[i] = i;
					centipedey[i] = 1;
					direction[i] = 1;
					collision[i] = 0;
					newX[i] = 0;
					endcounter = 0; 
					timer=0;
					score=0;
				}
			}
			if (userInput=='n'){
				cont = 0;
			}
		}
	}
}
