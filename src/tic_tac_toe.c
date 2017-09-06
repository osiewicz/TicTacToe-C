//Minmax implementation based on :https://gist.github.com/MatthewSteel/3158579#file-ttt-c-L8
#include <game_strings.h>
#include <tic_tac_toe.h>

int main(int argc,char *argv[])
{
	struct settings *settings = parse_cmd_args(argc,argv);
	main_menu(settings);
	win_check(NULL,settings->board_size,0);
	free_settings(settings);
	return 0;
}

void main_menu(struct settings *settings)
{
	char menu_choice[3];
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
}

int game(int AI_on,const struct settings *settings)
{
	char buffer[5], *p2_name, *p1_name;
	char *board = malloc(sizeof(char)*settings->board_size*settings->board_size);
	int response = 0;
	char current_player = 0;//default state. 1 for 'x', -1 for 'o'
	int turn_count;
	int input = 0;
	int game_state = 0;

	memset(board,0,settings->board_size*settings->board_size);

	current_player = get_random_value();
	current_player = (current_player % 2 == 0? 1:-1);

	p1_name = settings->p1_name;
	p2_name = (AI_on == 1 ? settings->AI_name : settings->p2_name);

	printf("%s\n",settings->game_strings[1]);
	print_board(board,settings->board_size,settings->p1_sign,settings->p2_sign,
			settings->ff_sign);

	printf("%s%s\n", ((current_player==-1) ? p1_name : p2_name), settings->game_strings[2]);

	for(turn_count = 0;turn_count < settings->board_size * settings->board_size ;turn_count++)/*Main gameplay loop*/
	{
		if (is_ai_turn(settings->ai_vs_ai,current_player,AI_on) == 1){
			input = minimax(current_player,board,settings->board_size, settings->board_size+5,-100000,100000,current_player);
		}else{
			printf("%s%s\n", ((current_player == -1) ? p1_name : p2_name),settings->game_strings[3]);
			fgets(buffer, sizeof(buffer), stdin);
			input = strtol(buffer,NULL,10);
			memset(buffer,0,5);
		}
		response = take_spot(input, current_player,board,settings->board_size);
		while(response == -1 && (AI_on == 0 || current_player == -1)) {
			printf("%s\n", settings->game_strings[4]);
			fgets(buffer, sizeof(buffer), stdin);
			input = strtol(buffer,NULL,10);
			response = take_spot(input, current_player,board,settings->board_size);
		}
		printf("%s %s %d!\n",(current_player == -1 ? p1_name : p2_name),settings->game_strings[5],input);
		print_board(board,settings->board_size,settings->p1_sign,settings->p2_sign,
				settings->ff_sign);
		game_state = win_check(board,settings->board_size,current_player);
		current_player = -current_player;
		if(  (game_state == 1 || game_state == -1)){
			printf("%s %s\n", (game_state == -1) ? p1_name : p2_name,settings->game_strings[6]);
			break;
		}
	}

	if(game_state == 0 && fields_count(board,settings->board_size,0)==0){
		printf("%s\n",settings->game_strings[7]);
	}
	free(board);

	return game_state;
}

int get_random_value(void)
{
	time_t tt;
	int value = 0;
	value = time(&tt);
	srand(value);
	value = rand();
	return value;
}

int take_spot(int field, int current_player,char *board,int board_size)
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
	i+=board_size;
	for(int j=0;j<board_size;j++){
		wins[i][j]=board_size*j+j;
	}
	i++;
	for(int j=0;j<board_size;j++){
		wins[i][j]=board_size*(board_size-j-1)+j;
	}
	return wins;
}

int free_wins(unsigned **wins, int board_size)
{
	if(board_size == 1){
		free(wins[0]);
	}
	else{
		for(int i=0;i<board_size*2+2;i++){
			free(wins[i]);
		}
	}
	free(wins);
	return 0;
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

int is_ai_turn(int ai_vs_ai, int current_player,int ai_on)
{
	return (ai_vs_ai == 1 || current_player == 1) && ai_on==1?1:0;
}

int win_check(char *board,int board_size,int current_player)
{
	int i,p;
	static unsigned **wins = NULL;
	if(wins == NULL){
		wins = wins_generator(board_size);
	}
	if(board == NULL && current_player == 0){
		free_wins(wins,board_size);
		wins = NULL;
		return -2;
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
	int i;
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

struct settings *init_settings(void)
{
	struct settings *result = malloc(sizeof(struct settings));
	result->ai_vs_ai = 0;
	result->board_size = 3;
	result->ff_sign = '#';
	result->language = "en-gb";
	result->p1_name = "Player 1";
	result->p1_sign = 'X';
	result->p2_name = "Player 2";
	result->p2_sign = 'O';
	result->AI_name = "AI";

	return result;
}

int free_settings(struct settings *settings)
{
	for(int i=0;i<settings->string_count;i++){
		free(settings->game_strings[i]);
	}
	free(settings->game_strings);
	free(settings);
	return 0;
}
int is_argument(char *argument)
{
	return argument == NULL || strncmp(argument,"--",2) == 0 ?0:1;
}

int is_valid_parameter(char *parameter,char *long_form, char *short_form)
{
	int short_result = 1;
	int long_result = 1;
	if(long_form!=NULL){
		long_result = strcmp(parameter,long_form);
	}
	if(short_form != NULL){
		short_result = strcmp(parameter,short_form);
	}
	return (long_result == 0 || short_result == 0)?1:0;
}

struct settings *parse_cmd_args(int argc, char *argv[]) {
	struct settings *result = init_settings();
	for(int i = 1; i < argc; i++){
		if( strncmp(argv[i],"-",1) == 0){
			if(argc - i > 1 && (is_valid_parameter(argv[i],"--language","-l") == 1
					&& is_argument(argv[i+1])==1)) {
				result->language = argv[i+1];
				i++;
			} else if(argc - i > 1 && is_valid_parameter(argv[i],"--p1_name",NULL) == 1
					&& is_argument(argv[i+1]) == 1) {
				result->p1_name = argv[i+1];
				i++;
			} else if(argc - i > 1 && is_valid_parameter(argv[i],"--p2_name",NULL) == 1
					&& is_argument(argv[i+1])==1 ) {
				result->p2_name = argv[i+1];
				i++;
			} else if(argc - i > 1 && is_valid_parameter(argv[i],"--AI_name",NULL) == 1
					&& is_argument(argv[i+1])==1 ) {
				result->AI_name = argv[i+1];
				i++;
			} else if(argc - i > 1 && is_valid_parameter(argv[i],"--b_size","-s") == 1
						&& is_argument(argv[i+1]) == 1) {
					result->board_size = strtol(argv[i+1],NULL,10);
					if(result->board_size < 2){
						result->board_size = 3;
					}
				i++;
			} else if(argc - i > 1 && (is_valid_parameter(argv[i],"--p1_sign",NULL) == 1) &&
					strlen(argv[i+1]) == 1){
				result->p1_sign = argv[i+1][0];
				i++;
			} else if(argc - i > 1 && (is_valid_parameter(argv[i],"--p2_sign",NULL) == 1) &&
					strlen(argv[i+1]) == 1){
				result->p2_sign = argv[i+1][0];
				i++;
			} else if(argc - i > 1 && is_valid_parameter(argv[i],"--ff_sign",NULL) == 1 &&
					strlen(argv[i+1]) == 1){
				result->ff_sign = argv[i+1][0];
				i++;
			} else if(argc - i > 0 && is_valid_parameter(argv[i],"--ai_vs_ai",NULL) == 1){
				result->ai_vs_ai = 1;
			} else {
				printf("%s: Unknown command-line parameter: %s\n",argv[0],argv[i]);
			}
		}  else {
				printf("%s: Unknown command-line argument: %s\n",argv[0],argv[i]);
		}
	}
	result->string_count = load_strings(result->language,&result->game_strings);
	return result;
}

