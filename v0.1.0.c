//Minmax implementation based on :https://gist.github.com/MatthewSteel/3158579#file-ttt-c-L8
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "settings.c"
#include "game_strings.c"

char board[3][3]; /* This array is responsible for storing current gamestate.*/
typedef enum {false, true} bool;

void print_board(void);
int move(char, int);
int win_condition_check(void);
void game(int player_choice);
void ai_analyze_board_state(void);
int minimax(int);

int main()
{
	char buffer[3];
	int player_choice;
	char language[5];
	strcpy(language,settings(2));
	const char **game_strings = strings(language);
	printf("%s\n",game_strings[0]);
	printf("%s\n",game_strings[1]);
	do{
		fgets(buffer, sizeof(buffer), stdin);
		player_choice = strtol(buffer,NULL,10);
	}	while(player_choice != 1 && player_choice != 2);
	/*Do-while loop presented above makes sure that end-user won't leave
	 *start menu unless he choses a valid option. strtol is used as a further
	 *security measure - since our strings are converted to valid numbers via
	 *function and not some gimmicky ASCII substraction there is certain level
	 *of comfort about malicious input from user.
	*/
	game(player_choice);
	return 0;
}

void game(int player_choice)/*Game loop logic*/
{
	int ai_level,seed,response = 0;//buffer for values returned by functions
	int current_player = 0;//default state. 1 for 'x', -1 for 'o'
	char buffer[20],p1[100],p2[100],language[5]; //Player names and buffer for input.
	time_t tt;
	int i,p;//loop controlers
	int input = 0;
	/*User input will be transformed from string form (buffer)
	 *to integer. Just a safety measure.
	 */
	strcpy(language,settings(2));
	const char **game_strings = strings(language);
	ai_level = *settings(3);
	strcpy(p1,settings(4));
	strcpy(p2,settings(5));

	while(1){
		seed = time(&tt);/*Time passed since 01.01.1970 is used as randomizer seed*/
		srand(seed);
		if(player_choice == 1){
			strncpy(p2,"PC", sizeof(p2)-1);
		}
		for(i = 0;i<3;i++){
			for(p = 0;p<3;p++)
				board[i][p] = '#';
		}/* Clear the board*/
		printf("%s\n",game_strings[2]);
		print_board();
		if(seed%2 == 0){
			current_player =1;
		}else{
			current_player =-1;
		}
		printf("%s%s\n", current_player==-1?p1:p2,game_strings[3]);
		for(i = 0;i<9;i++)/*Main gameplay loop*/
		{
			if(player_choice == 2){
			printf("%s%s\n", current_player == 1?p1:p2,game_strings[4]);
			fgets(buffer, sizeof buffer, stdin);
			input = strtol(buffer,NULL,10);
			response = move(input, current_player);
			while(response == -1) {
				printf("%s\n",game_strings[5]);
				fgets(buffer, sizeof buffer, stdin);
				input = strtol(buffer,NULL,10);
				response = move(input, current_player);
			}
			/*response holds the return of move function, which returns 0 on success
			* and -1 on failure - code below is rather repeptitive and a pattern can
			* be seen at glance - if move() returned error, we'll repeat move until
			* we get expected  response
			*/
		} else if(current_player == -1 && player_choice == 1){
				printf("%s%s\n",p1,game_strings[4]);
				fgets(buffer, sizeof buffer, stdin);
				input = strtol(buffer,NULL,10);
				response = move(input, current_player);
				while(response == -1) {
					printf("%s\n",game_strings[5]);
					fgets(buffer, sizeof buffer, stdin);
					input = strtol(buffer,NULL,10);
					response = move(input, current_player);
				}
			} else if(current_player == 1){
					printf("%s ",game_strings[6]);
					ai_analyze_board_state();
}
				/*We still want to check whether PC didn't try to occupy occupied field,
				however there is no need to print out a warning*/
				print_board();
				current_player = current_player*(-1);//Turn is about to end, so turn bool's value is reversed
				if(i >= 4 && (win_condition_check() == 1 || win_condition_check() == -1)){
					printf("%s %s\n", win_condition_check() == -1?p1:p2,game_strings[7]);
					break;
				}/*win_condition_check checks the state of the board and if it returns player_id,
					* then we finish the game and print out winner's nickname*/
			}
			if(win_condition_check() == 0)
			printf("%s\n",game_strings[8]);
			/*win_condition_check returns 0 when noone is winner in current turn.
			 *We check for this specific return only at the end of the whole game,
			 *as it means that game has ended in a draw.
			 */
		printf("%s\n",game_strings[9]);
		fgets(buffer, 3, stdin);
		buffer[strcspn(buffer, "\n")] = 0;
		if(buffer[0] == 'Y'||buffer[0] == 'y'){
			continue;
		}
		if(buffer[0] == 'N'||buffer[0] == 'n'){
			break;
		}else {
			continue;
		}
	}
}

int move(char pole, int current_player)
{
	int i,p;
	for(i = 0;i<3;i++){
		for(p = 0;p<3;p++){
			if(pole-1 == i*3+p)
				if (board[i][p] == '#'){
					board[i][p] = (current_player)==-1 ? 'X' : 'O'; return 0;
				}
	/*If field is not occupied(its value is hashtag), take up that field.
	Otherwise return error which can be processed by loop in game()*/
		}
	}
	return -1;
}

void print_board(void)
{
	printf("%c|%c|%c\n%c|%c|%c\n%c|%c|%c\n \n", board[0][0], board[0][1], board[0][2],
	board[1][0], board[1][1], board[1][2], board[2][0], board[2][1], board[2][2]);
}
int win_condition_check()
{
	int i,p;
	unsigned wins[8][3][2] = {{{0,0},{0,1},{0,2}},{{1,0},{1,1},{1,2}},{{2,0},{2,1},{2,2}},{{0,0},{1,0},{2,0}},
	{{0,1},{1,1},{2,1}},{{0,2},{1,2},{2,2}},{{0,0},{1,1},{2,2}},{{0,2},{1,1},{2,0}}};
	for(i=0;i<8;i++){
		for(p=0;p<3;p++){
			if(board[wins[i][p][0]][wins[i][p][1]]=='X'&&p==2)
				return -1;
			else if(board[wins[i][p][0]][wins[i][p][1]]=='X')
				continue;
			else
				break;
		}
	}
	for(i=0;i<8;i++){
		for(p=0;p<3;p++){
			if(board[wins[i][p][0]][wins[i][p][1]]=='O'&&p==2)
				return 1;
			else if(board[wins[i][p][0]][wins[i][p][1]]=='O')
				continue;
			else
				break;
		}
	}

				return 0;
}
void ai_analyze_board_state(){
	int move_i=-1,move_j=-1;
	int score=-2;
	int i,j;
	for(i=0;i<3;++i){
		for(j=0;j<3;++j){
			if(board[i][j]=='#'){
				board[i][j]='O';
				int tempScore=-minimax(-1);
				board[i][j]='#';
				if(tempScore>score){
					score=tempScore;
					move_i=i;
					move_j=j;
				}

			}
		}
	}
	printf(" %d!\n",3*move_i+move_j+1);
	board[move_i][move_j]='O';
}
int minimax(int player){
	int winner=win_condition_check();
	if(winner!=0)
		return winner*player;
	int move = -1;
	int score = -2;
	int i,j;
	for(i=0;i<3;++i){
		for(j=0;j<3;++j){
			if(board[i][j]=='#'){
				board[i][j]=(player==-1?'X':'O');
				int tempScore=-minimax(-1*player);
				if(tempScore>score){
					score=tempScore;
					move=3*i+j;
				}
				board[i][j]='#';
			}
		}
	}
	return move==-1?0:score;
}
