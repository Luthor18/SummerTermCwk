#include <ncurses.h> //this is used for ncurses
#include <unistd.h>
#include <time.h> //this is used for random number generation
#include <stdlib.h>
#define DELAY 30000

int main(int argc, char *argv[]) {
	int randomnumber, i = 0, c=0, t=0, cont = 1, endcounter = 0, timer=0, score=0, gamestate = 1; //counter and number ints
	int newX[8] = {0};
	int centipedex[8] = {0, 1, 2, 3, 4, 5, 6, 7 }, centipedey[8] = {1, 1, 1, 1, 1, 1, 1, 1}, direction[8] = {1, 1, 1, 1, 1, 1, 1, 1}, collision[8] = {0}; //centipede variables and values
	int shipx, shipy; //ship and projectile variables
	int projx, projy, projFired=0;  //declare all misc variables here
	int max_y = 0, max_x = 0;
	char userInput;
	
	srand(time(NULL));
	initscr();    //initialise the screen, random numbers etc here
	noecho();
	curs_set(FALSE);
	getmaxyx(stdscr, max_y, max_x); //get the screen size
	
	int mushrooms[max_y][max_x];
	for (i=0;i<max_y;i++){
		for(c=0;c<max_x;c++){
			mushrooms[i][c] = 0; //clear the array for mushrooms
		}
	}
	for (i=2;i<max_y-1;i+=3){
		randomnumber = rand() % (max_x-6);
		mushrooms[i][3+randomnumber] = 2;
		randomnumber = rand() % (max_x-6); //print mushrooms on every 3 lines at random intervals
		mushrooms[i][3+randomnumber] = 2;
	}
		
	shipx = max_x/2;
	shipy = max_y-1; //place the ship at the bottom middle of screen
	
	nodelay(stdscr, TRUE); //stop the program from delaying while waiting for an input
	
	while(cont == 1){	//while the user has chosen to keep playing
		while(gamestate==1) { //while the game state is in 'play' mode
			timer++;
			getmaxyx(stdscr, max_y, max_x); //increment timer by 1, get screen size (incase its resized)
			clear();
			userInput = getch(); //see if the user inputs a char
			
			
			for (i=0;i<8;i++){
				newX[i] = centipedex[i]+direction[i];       //this for loop checks for any catapillar collisions
				if (newX[i]>=max_x || newX[i]<0){ //if it collides with the edge of the stage
					centipedey[i]+=1;
					direction[i]*= -1;
				}
				else{
					if (mushrooms[centipedey[i]][newX[i]]!=0){ //if it collides with a mushroom
						centipedey[i]+=1;
						direction[i]*= -1;
					}
					else{
						centipedex[i]+=direction[i]; //if it doesnt, just keep it moving in the same direction
					}
					
				}
				
				if (collision[i] ==0 ){
					mvprintw(centipedey[i], centipedex[i], "o"); //print out a n o for each piece that hasnt been hit
				}
				if (centipedex[i] == shipx){
					if (centipedey[i] == shipy){ //if the ship gets hit by the catapillar, display game over
						gamestate=0;
					}
				}
			}
			if (userInput == 'a'){
				if (!((shipx-1)<=0)){ //if the user presses a move left
					shipx--;
				}
			}
			if (userInput == 'd'){
				if (!((shipx+1)>=max_x)){ //if the user presses d move right
					shipx++;
				}
			}
			if (projFired == 0){
				if (userInput == 'w'){	//if the user presses w fire a projectile
				projFired = 1;
				projx=shipx;
				projy=shipy;
				}
			}
			
			if (projFired ==1){
				if (projy<=0){
					projy=shipy; //if the projectile goes off the screen reset it
					projFired=0;
				}
				for (i=0;i<8;i++){
					if (projy==centipedey[i]&&projx==centipedex[i]){ //if it hits a piece of the centipede have it dissapear
						if (collision[i]==0){
							collision[i] = 1;
							score+=10;
							endcounter++;
						}
						
					}
				}
				if (mushrooms[projy][projx] == 2){ //if it hits a mushroom have it dissapear and increase score
					mushrooms[projy][projx] = 1;
					projFired = 0;
					score++;
				}
				else if (mushrooms[projy][projx] == 1){ //same with the little mushrooms
					mushrooms[projy][projx] = 0;
					projFired = 0;
					score++;
				}
				
				projy--;
				mvprintw(projy, projx, "*"); //print the projectile here
			}
			
			for (i=0;i<max_y;i++){
				for (c=0;c<max_x;c++){
					if (mushrooms[i][c] == 2){ //print the mushroom field here (with updated hitscanning)
						mvprintw(i, c, "X");
					}
					if (mushrooms[i][c] == 1){
						mvprintw(i, c, "+");
					}
				}
			}
			
			shipy = max_y-1;
			mvprintw(shipy, shipx, "W");
			mvprintw(0,0,"Timer: %d", timer/30); //print the ship (always at the bottom) here and the score and timer
			mvprintw(0, 10, "Score: %d", score);
			if (endcounter==8){
				gamestate = 2; //detect if the game has ended or not
			}	
			refresh();
			usleep(DELAY);
			userInput = 't'; //refresh and reset, set user input to t so to not keep loops infinitely going
			endwin();
		}
		if (gamestate == 0){
			getmaxyx(stdscr, max_y, max_x);
			clear();
			mvprintw(max_y/2, (max_x/2)-5, "GAME OVER! YOU LOSE!");
			mvprintw((max_y/2)+1, (max_x/2)-5, "Score: %d    You survived for %d seconds.", score, timer/30); //this large chunk of code is for the game over state
			mvprintw((max_y/2)+2, (max_x/2)-5, "Play again? Yes or no?(y or n)"); //print the scores and ask if they want to play again
			refresh();
			userInput = getch(); //y means play again, n means quit
			usleep(1000);
			if (userInput=='y'){
				gamestate = 1;
				for (i=0;i<8;i++){
					centipedex[i] = i;
					centipedey[i] = 1; //this code resets necessary variables if they want to play again
					direction[i] = 1;
					collision[i] = 0;
					newX[i] = 0;
					endcounter = 0; 
					timer=0;
					score=0;
				}
			}
			if (userInput=='n'){
				cont = 0; //this closes the program
				clear();
				endwin();
			}
		}
		
		if (gamestate == 2){
			getmaxyx(stdscr, max_y, max_x);
			clear();
			mvprintw(max_y/2, (max_x/2)-5, "CONGRATULATIONS! YOU WIN!");
			mvprintw((max_y/2)+1, (max_x/2)-5, "Score: %d    You survived for %d seconds.", score, timer/30); //this code is a repeat of the previous block but if they win.
			mvprintw((max_y/2)+2, (max_x/2)-5, "Play again? Yes or no?(y or n)");
			refresh();
			userInput = getch();
			usleep(1000);
			if (userInput=='y'){
				gamestate = 1;
				for (i=0;i<8;i++){
					centipedex[i] = i;
					centipedey[i] = 1; //this code resets necessary variables
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
				clear();
				endwin();
			}
		}
	}
}
