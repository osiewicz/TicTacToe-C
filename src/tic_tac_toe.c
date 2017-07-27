//Minmax implementation based on :https://gist.github.com/MatthewSteel/3158579#file-ttt-c-L8
#include <game_strings.h>
#include <tic_tac_toe.h>

char board[3][3];
static struct game_settings *init_game_settings()
{
	struct game_settings *result = malloc(sizeof(struct game_settings));
	result->language = malloc(sizeof(char) * BUFF_SIZE);
	result->p1_name = malloc(sizeof(char) * BUFF_SIZE);
	result->p2_name = malloc(sizeof(char) * BUFF_SIZE);
	result->AI_name = malloc(sizeof(char) * BUFF_SIZE);
	result->game_strings = NULL;
	memset(result->language,0,BUFF_SIZE);
	memset(result->p1_name,0,BUFF_SIZE);
	memset(result->p2_name,0,BUFF_SIZE);
	memset(result->AI_name,0,BUFF_SIZE);
}

int main(int argc,char *argv[])
{
	char *menu_choice = malloc(sizeof(char)*3);

	struct game_settings *settings = parse_cmd_args(argc,argv);

	printf("TicTacToe v%s\n",TTT_VERSION);
	do{
		printf("%s\n",settings->game_strings[0]);
		do{
			fgets(menu_choice,3,stdin);
		}while(*menu_choice != '1' && *menu_choice != '2');
		game((*menu_choice)-'0',settings);

		printf("%s\n", settings->game_strings[8]);
		fgets(menu_choice,3,stdin);
	} while(*menu_choice == 'Y' || *menu_choice == 'y');

	free(settings);
	free(menu_choice);

	return 0;
}

void game(int player_choice,const struct game_settings *settings)
{
	char buffer[20], *p2_name, *p1_name;
	int seed,response = 0;
	char current_player = 0;//default state. 1 for 'x', -1 for 'o'
	int turn_count;
	int input = 0;
	time_t tt;

	p1_name = settings->p1_name;
	p2_name = (player_choice == 1 ? settings->AI_name : settings->p2_name);

	seed = time(&tt);/*Time passed since 01.01.1970 is used as randomizer seed*/
	srand(seed);

	/* reset board */
	for(int i = 0;i<3;i++){
		for(int p = 0;p<3;p++)
			board[i][p] = '#';
	}

	printf("%s\n",settings->game_strings[1]);
	print_board();

	current_player = (seed % 2 == 0? 1:-1);
	printf("%s%s\n", ((current_player==-1) ? p1_name : p2_name), settings->game_strings[2]);

	for(turn_count = 0;turn_count<9;turn_count++)/*Main gameplay loop*/
	{
		if (current_player == 1 && player_choice == 1){
			input = ai_analyze_board_state();
		}else{
			printf("%s%s\n", ((current_player == -1) ? p1_name : p2_name),settings->game_strings[3]);
			fgets(buffer, sizeof buffer, stdin);
			input = strtol(buffer,NULL,10);
		}
		response = move(input, current_player);
		while(response == -1) {
			printf("%s\n", settings->game_strings[4]);
			fgets(buffer, sizeof buffer, stdin);
			input = strtol(buffer,NULL,10);
			response = move(input, current_player);
		}
		printf("%s %s %d!\n",(current_player == -1 ? p1_name : p2_name),settings->game_strings[5],input);
		print_board();
		current_player = current_player*(-1);//Turn is about to end, so turn bool's value is reversed
		int game_state = x3_win_check();
		if(turn_count >= 4 && (game_state == 1 || game_state == -1)){
			printf("%s %s\n", (game_state == -1) ? p1_name : p2_name,settings->game_strings[6]);
			break;
		}
	}

	if(x3_win_check() == 0) {
		printf("%s\n",settings->game_strings[7]);
	}
}

int move(char field, int current_player)
{
	if(field <= 0 || field > 9){
		return -1;
	}
	for(int i = 0;i<3;i++){
		for(int p = 0;p<3;p++){
			if (field-1 == i*3+p && board[i][p] == '#') {
				board[i][p] = ( (current_player) == -1 ? 'X' : 'O' );
				return 0;
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

int x3_win_check(void)
{
	int i,p;
	const unsigned wins[8][3][2] = {{{0,0}, {0,1}, {0,2}}, {{1,0}, {1,1}, {1,2}},
		{{2,0}, {2,1}, {2,2}}, {{0,0}, {1,0},{2,0}}, {{0,1}, {1,1}, {2,1}},
		{{0,2}, {1,2}, {2,2}}, {{0,0}, {1,1}, {2,2}}, {{0,2}, {1,1}, {2,0}}};
	for(i=0;i<8;i++){
		for(p=0;p<3;p++){
			if (board[ wins[i][p][0] ][ wins[i][p][1] ] == 'X' && p == 2) {
				return -1;
			} else if(board[ wins[i][p][0] ][ wins[i][p][1] ] == 'X') {
				continue;
			} else {
				break;
			}
		}
	}
	for(i=0;i<8;i++){
		for(p=0;p<3;p++){
			if(board[ wins[i][p][0] ][ wins[i][p][1] ] == 'O' && p == 2) {
				return 1;
			} else if(board[ wins[i][p][0] ][ wins[i][p][1] ] == 'O') {
				continue;
			} else {
				break;
			}
		}
	}
	return 0;
}

int ai_analyze_board_state(void){
	int move_i = -1, move_j = -1;
	int score = -2;
	for(int i=0;i<3;++i){
		for(int j=0;j<3;++j){
			if(board[i][j] == '#') {
				board[i][j] = 'O';
				int tempScore = -minimax(-1);
				board[i][j] = '#';
				if(tempScore > score){
					score = tempScore;
					move_i = i;
					move_j = j;
				}
			}
		}
	}
	return 3 * move_i + move_j + 1;
}

int minimax(int player){
	int winner = x3_win_check();
	if (winner != 0) {
		return winner*player;
	}
	int move = -1;
	int score = -2;
	int i,j;
	for(i=0;i<3;++i){
		for(j=0;j<3;++j){
			if( board[i][j] == '#'){
				board[i][j] = (player == -1 ? 'X' : 'O');
				int tempScore = -minimax(-1*player);
				if(tempScore > score){
					score = tempScore;
					move = 3 * i + j;
				}
				board[i][j] = '#';
			}
		}
	}
	return move == -1 ? 0 : score;
}

struct game_settings *parse_cmd_args(int argc, char *argv[]) {
	struct game_settings *result = init_game_settings();
	result->language = "en-gb";
	result->p1_name = "Player 1";
	result->p2_name = "Player 2";
	result->AI_name = "AI";
	for(int i = 1; i < argc; i++){
		if( strncmp(argv[i],"-",1) == 0){
			if(argc - i > 1 && (strcmp(argv[i],"-l") == 0 ||
				strcmp(argv[i],"--language") == 0) && strncmp(argv[i+1],"--",2) != 0) {
				result->language = argv[i+1];
				i++;
			} else if(argc - i > 1 && strcmp(argv[i],"--p1_name") == 0
					&& strncmp(argv[i+1],"--",2) != 0 ) {
				result->p1_name = argv[i+1];
				i++;
			} else if(argc - i > 1 && strcmp(argv[i],"--p2_name") == 0
					&& strncmp(argv[i+1],"--",2) != 0 ) {
				result->p2_name = argv[i+1];
				i++;
			} else if(argc - i > 1 && strcmp(argv[i],"--AI_name") == 0
					&& strncmp(argv[i+1],"--",2) != 0 ) {
				result->AI_name = argv[i+1];
				i++;
			} else {
				printf("%s: Unknown command-line parameter: %s\n",argv[0],argv[i]);
			}
		}  else {
				printf("%s: Unknown command-line argument: %s\n",argv[0],argv[i]);
		}
	}
	result->game_strings = (char**)strings(result->language);
	return result;
}
