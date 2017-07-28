#pragma once

#define TTT_VERSION "1.0.0"
#define BUFF_SIZE 256
#define _GNU_SOURCE
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct game_settings{
	char *language;
	char *p1_name;
	char *p2_name;
	char *AI_name;
	char **game_strings;
	int board_size;
	int (*win_check)(char **board);
};

int ai_analyze_board_state(char **board,int board_size,int (*win_check)(char **board));
void game(int player_choice,const struct game_settings *settings);
int minimax(int player,char **board,int board_size,int (*win_check)(char **board));
int move(char field, int current_player,char **board,int board_size);
void print_board(char **board,int board_size);
int x3_win_check(char **board);
struct game_settings *parse_cmd_args(int argc, char *argv[]);

