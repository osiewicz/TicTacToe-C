//Minmax implementation based on :https://gist.github.com/MatthewSteel/3158579#file-ttt-c-L8
#include <game_strings.h>
#include <tic_tac_toe.h>

static struct game_settings *init_game_settings()
{
	struct game_settings *result = malloc(sizeof(struct game_settings));
	result->AI_name = malloc(sizeof(char) * BUFF_SIZE);
	result->language = malloc(sizeof(char) * BUFF_SIZE);
	result->p1_name = malloc(sizeof(char) * BUFF_SIZE);
	result->p2_name = malloc(sizeof(char) * BUFF_SIZE);
	result->game_strings = NULL;
	result->board_size = 0;
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
		game((*menu_choice)-'1',settings);

		printf("%s\n", settings->game_strings[8]);
		fgets(menu_choice,3,stdin);
	} while(*menu_choice == 'Y' || *menu_choice == 'y');

	free(settings);
	free(menu_choice);

	return 0;
}

int game(int AI_on,const struct game_settings *settings)
{
	char buffer[5], *p2_name, *p1_name;
	char *board = malloc(sizeof(char)*settings->board_size*settings->board_size);
	int response = 0;
	char current_player = 0;//default state. 1 for 'x', -1 for 'o'
	int turn_count;
	int input = 0;
	int game_state = 0;
	time_t tt;

	memset(board,0,settings->board_size*settings->board_size);

	current_player = time(&tt);/*Time passed since 01.01.1970 is used as randomizer seed*/
	srand(current_player);
	current_player = (current_player % 2 == 0? 1:-1);

	p1_name = settings->p1_name;
	p2_name = (AI_on == 1 ? settings->AI_name : settings->p2_name);

	printf("%s\n",settings->game_strings[1]);
	print_board(board,settings->board_size,settings->p1_sign,settings->p2_sign,
			settings->ff_sign);

	printf("%s%s\n", ((current_player==-1) ? p1_name : p2_name), settings->game_strings[2]);

	for(turn_count = 0;turn_count < settings->board_size * settings->board_size ;turn_count++)/*Main gameplay loop*/
	{
		if ((settings->ai_vs_ai == 1 || current_player == 1) && AI_on == 1){
			input = minimax(current_player,board,settings->board_size, settings->board_size+5,-100000,100000,current_player);
		}else{
			printf("%s%s\n", ((current_player == -1) ? p1_name : p2_name),settings->game_strings[3]);
			fgets(buffer, sizeof(buffer), stdin);
			input = strtol(buffer,NULL,10);
			memset(buffer,0,5);
		}
		response = move(input, current_player,board,settings->board_size);
		while(response == -1 && (AI_on == 0 || current_player == -1)) {
			printf("%s\n", settings->game_strings[4]);
			fgets(buffer, sizeof(buffer), stdin);
			input = strtol(buffer,NULL,10);
			response = move(input, current_player,board,settings->board_size);
		}
		printf("%s %s %d!\n",(current_player == -1 ? p1_name : p2_name),settings->game_strings[5],input);
		print_board(board,settings->board_size,settings->p1_sign,settings->p2_sign,
				settings->ff_sign);
		game_state = win_check(board,settings->board_size,current_player);
		current_player = -current_player;//Turn is about to end, so turn bool's value is reversed
		if(  (game_state == 1 || game_state == -1)){
			printf("%s %s\n", (game_state == -1) ? p1_name : p2_name,settings->game_strings[6]);
			break;
		}
	}

	if(game_state == 0 &&win_check(board,settings->board_size,current_player) == 0) {
		printf("%s\n",settings->game_strings[7]);
	}
	free(board);

	return game_state;
}

int move(char field, int current_player,char *board,int board_size)
{
	if(field <= 0 || field > board_size * board_size){
		return -1;
	}

	field--; //Standarize input from 1-based fields to 0-based
	if (board[field] == 0) {
		board[field] = current_player;
		return 0;
	}
	
	return -1;
}

void print_board(char *board, int board_size,char p1_sign,char p2_sign,char ff_sign)
{
	for(int i = 0; i < board_size*board_size ; i++) {
			switch(board[i]){
				case -1:
					printf("%c",p1_sign);
					break;
				case 1:
					printf("%c",p2_sign);
					break;
				default:
					printf("%c",ff_sign);
					break;
			}
			if((i+1) % board_size == 0) {
				printf("\n");
			} else {
				printf("|");
			}
	}
	printf("\n");
}

unsigned **wins_generator(int board_size){
	int wins_count = (board_size == 1 ? 1 : 2*board_size + 2);
	unsigned **wins = malloc(sizeof(unsigned*) * wins_count);
	for(int i=0;i<wins_count;i++){
		wins[i] = malloc(sizeof(unsigned) * board_size);
	}

	int i,t=0;
	for(i = 0; i < board_size; i++){
		for(int j = 0; j <board_size;j++){
			wins[i][j] = board_size*j+t;
			wins[i+board_size][j] = board_size*t+j;
		}
		t++;
	}
	t = 0;
	for(; i < 2*board_size; i++){
		for(int j = 0; j <board_size;j++){
			wins[i][j] = board_size*t+j;
		}
		t++;
	}
	for(int j=0;j<board_size;j++){
		wins[i][j]=board_size*j+j;
	}
	i++;
	for(int j=0;j<board_size;j++){
		wins[i][j]=board_size*(board_size-j-1)+j;
	}
	return wins;
}

void printf_wins(unsigned **wins,int board_size)
{
	/* For debugging purposes only */
	for(int i = 0;i<2*board_size+2;i++){
		for(int j=0;j<board_size;j++){
			printf("%u|",wins[i][j]);
		}
		printf("\n");
	}
}

int fields_count(char *board,int board_size,int field)
{
	int count = 0;
	for(int i = 0;i<board_size*board_size;i++){
		if(board[i] == field){
			count++;
		}
	}
	return count;
}

int win_check(char *board,int board_size,int current_player)
{
	int i,p;
	static unsigned **wins = NULL;
	if(wins == NULL){
		wins = wins_generator(board_size);
	}
	for(i = 0;i<2*board_size+2;i++){
		for(p=0;p<board_size;p++){
			if(board[wins[i][p]] == current_player && p == board_size - 1) {
				return current_player;
			} else if(board[wins[i][p]] != current_player) {
				break;
			}
		}
	}
	return 0;
}

int minimax(int player,char *board,int board_size,int depth,int alpha,int beta,int init_player)
{
	int winner = win_check(board,board_size,player);
	if(depth == 0 || (winner == 0 && fields_count(board,board_size,0) == 0) ){
		return 0;
	}
	if (winner != 0 && depth != board_size+5) {
		return (board_size*board_size-depth)*(winner==init_player?1:-1);
	}
	int i,j;
	int move = -1;
	int MyMove = 0;
	int MyScore;
	int score = -200000;
	for(i=0;i<board_size*board_size;++i){
		if( board[i] == 0){
			board[i] = player;
			MyScore = minimax(-player,board,board_size,depth-1,alpha,beta,init_player);
			board[i] = 0;
			if(MyScore>=score&&depth ==board_size+5){
				MyMove = i;
				score = MyScore;
			}
			if(player == -init_player && depth!=board_size+5){
				beta = (MyScore > beta? beta: MyScore); 
			} else if(player == init_player && depth != board_size+5){
				alpha = (MyScore < alpha? alpha: MyScore);
			}

			if(beta <= alpha && depth != board_size+5) {
				return (player == init_player? alpha: beta);
			}
			move = i;

		}
	}
	return depth==board_size+5?MyMove+1:move==-1?0:(player == init_player? alpha: beta);
}

struct game_settings *parse_cmd_args(int argc, char *argv[]) {
	struct game_settings *result = init_game_settings();
	result->ai_vs_ai = 0;
	result->board_size = 3;
	result->ff_sign = '#';
	result->language = "en-gb";
	result->p1_name = "Player 1";
	result->p1_sign = 'X';
	result->p2_name = "Player 2";
	result->p2_sign = 'O';
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
			} else if(argc - i > 1 && (strcmp(argv[i],"--b_size") == 0 ||
					strcmp(argv[i],"-s") == 0) && strncmp(argv[i+1],"--",2) != 0 ) {
				if(result->board_size<2){
					result->board_size = strtol(argv[i+1],NULL,10);
				}
				i++;
			} else if(argc - i > 1 && (strcmp(argv[i],"--p1_sign") == 0) &&
					strlen(argv[i+1]) == 1){
				result->p1_sign = argv[i+1][0];
				i++;
			} else if(argc - i > 1 && (strcmp(argv[i],"--p2_sign") == 0) &&
					strlen(argv[i+1]) == 1){
				result->p2_sign = argv[i+1][0];
				i++;
			} else if(argc - i > 1 && (strcmp(argv[i],"--ff_sign") == 0) &&
					strlen(argv[i+1]) == 1){
				result->ff_sign = argv[i+1][0];
				i++;
			} else if(argc - i > 0 && (strcmp(argv[i],"--ai_vs_ai") == 0)){
				result->ai_vs_ai = 1;
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
