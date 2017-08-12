#pragma once

#define TTT_VERSION "1.4.0"
#define BUFF_SIZE 256
#define _GNU_SOURCE
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct game_settings{
	char *AI_name;
	char ff_sign;
	char **game_strings;
	char *language;
	char *p1_name;
	char p1_sign;
	char *p2_name;
	char p2_sign;
	int ai_vs_ai;
	int board_size;
};

int ai_analyze_board_state(char *board,int board_size,int current_player);
int game(int AI_on,const struct game_settings *settings);
int minimax(int player,char *board,int board_size,int depth,int alpha,int beta,int init_player);
int move(char field, int current_player,char *board,int board_size);
void print_board(char *board,int board_size,char p1_sign,char p2_sign,char ff_sign);
unsigned **win_generator(int board_size);
int win_check(char *board,int board_size,int current_player);
struct game_settings *parse_cmd_args(int argc, char *argv[]);

