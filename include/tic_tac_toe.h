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
};

int ai_analyze_board_state(void);
void game(int player_choice,const struct game_settings *settings);
int minimax(int player);
int move(char field, int current_player);
void print_board(void);
int x3_win_check(void);
struct game_settings *parse_cmd_args(int argc, char *argv[]);

