//Minmax implementation based on :https://gist.github.com/MatthewSteel/3158579#file-ttt-c-L8
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "settings.c"
#include "game_strings.c"

char board[3][3]; /* This array is responsible for storing current gamestate.*/

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
	 *start menu unless he choses a valid option.
	*/
	game(player_choice);
	return 0;
}

void game(int player_choice)/*Game loop function*/
{
	int seed,response = 0;//buffer for values returned by functions and starting seed
	int current_player = 0;//default state. 1 for 'x', -1 for 'o'
	char buffer[20],p1_name[100],p2_name[100],language[5];
	time_t tt;
	int turn_count;//turn count(loop iterator)
	int input = 0;
	strcpy(language,settings(2));
	const char **game_strings = strings(language);
	strcpy(p1_name,settings(4));
	player_choice==1?strncpy(p2_name,"PC", sizeof(p2_name)-1):strcpy(p2_name,settings(5));
	while(1){
		seed = time(&tt);/*Time passed since 01.01.1970 is used as randomizer seed*/
		srand(seed);

		for(int i = 0;i<3;i++){
			for(int p = 0;p<3;p++)
				board[i][p] = '#';
		}/* Clear the board*/
		printf("%s\n",game_strings[2]);
		print_board();
		current_player = seed%2 == 0? 1:-1;
		printf("%s%s\n", current_player==-1?p1_name:p2_name,game_strings[3]);
		for(turn_count = 0;turn_count<9;turn_count++)/*Main gameplay loop*/
		{
			if(current_player==1&& player_choice==1){
				printf("%s ",game_strings[6]);
				ai_analyze_board_state();
			}else{
				printf("%s%s\n", current_player == -1?p1_name:p2_name,game_strings[4]);
				fgets(buffer, sizeof buffer, stdin);
				input = strtol(buffer,NULL,10);
				response = move(input, current_player);
				while(response == -1) {
					printf("%s\n",game_strings[5]);
					fgets(buffer, sizeof buffer, stdin);
					input = strtol(buffer,NULL,10);
					response = move(input, current_player);
				}
		}
				print_board();
				current_player = current_player*(-1);//Turn is about to end, so turn bool's value is reversed
				if(turn_count >= 4 && (win_condition_check() == 1 || win_condition_check() == -1)){
					printf("%s %s\n", win_condition_check() == -1?p1_name:p2_name,game_strings[7]);
					break;
				}
			}
			if(win_condition_check() == 0)
				printf("%s\n",game_strings[8]);
		printf("%s\n",game_strings[9]);
		fgets(buffer, 3, stdin);
		buffer[strcspn(buffer, "\n")] = 0;
		if(buffer[0] == 'Y'||buffer[0] == 'y')
			continue;
		else
			break;
	}
}

int move(char pole, int current_player)
{
	for(int i = 0;i<3;i++){
		for(int p = 0;p<3;p++){
			if(pole-1 == i*3+p)
				if (board[i][p] == '#'){
					board[i][p] = (current_player)==-1 ? 'X' : 'O'; return 0;
				}
		}
	}
	return -1;
}

void print_board(void)
{
	printf("%c|%c|%c\n%c|%c|%c\n%c|%c|%c\n \n", board[0][0], board[0][1], board[0][2],
	board[1][0], board[1][1], board[1][2], board[2][0], board[2][1], board[2][2]);
}

int win_condition_check(void)
{
	int i,p;
	const unsigned wins[8][3][2] = {{{0,0},{0,1},{0,2}},{{1,0},{1,1},{1,2}},{{2,0},{2,1},{2,2}},{{0,0},{1,0},{2,0}},
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

void ai_analyze_board_state(void){
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
