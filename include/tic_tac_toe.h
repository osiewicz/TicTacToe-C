#pragma once

#define TTT_VERSION "1.7.0"
#define BUFF_SIZE 256
#define _GNU_SOURCE
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct settings{
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
	int string_count;
};

void main_menu(struct settings *settings);
int game(int AI_on,const struct settings *settings);
int get_random_value(void);
int take_spot(int field, int current_player,char *board,int board_size);
int fields_count(char *board,int board_size,int field);
void print_board(char *board,int board_size,char p1_sign,char p2_sign,char ff_sign);
unsigned **win_generator(int board_size);
int free_wins(unsigned **wins,int board_size);
void printf_wins(unsigned **wins,int board_size);
int is_ai_turn(int ai_vs_ai, int current_player,int ai_on);
int win_check(char *board,int board_size,int current_player);
int minimax(int player,char *board,int board_size,int depth,int alpha,int beta,int init_player);
struct settings *init_settings(void);
int free_settings(struct settings *settings);
int is_argument(char *argument);
int is_valid_parameter(char *parameter,char *long_form, char *short_form);
struct settings *parse_cmd_args(int argc, char *argv[]);
