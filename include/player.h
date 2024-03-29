#ifndef PLAYER_H_GUARD
#define PLAYER_H_GUARD

#include <string.h>
#include <time.h>
#include "cell.h"
#include "board.h"
#include "ncurses.h"
#include "config.h"
#include "boat.h"
#include "board_view.h"

typedef enum {HUMAN, IA} Player_type;

typedef struct Player Player;

struct Player {
	int boats_alive;
	void (*play) (WINDOW *win, Cell board[BOARD_SIZE][BOARD_SIZE], Player *human, Player *ia, int turn);
	Boat *boats[5];
	char history[100][2];
};

void player_factory (Player *player, Player_type type);

void new_player (Player *player, int boats_alive, void (*play)(WINDOW *win, Cell board[BOARD_SIZE][BOARD_SIZE], Player *human, Player *ia, int turn));

void hit (Cell *cell, Player *hit_player);

void ask_player_to_place_boats(WINDOW *win, Cell board[BOARD_SIZE][BOARD_SIZE], Player *player);

void ask_player_to_place_one_boat(WINDOW *win, Cell board[BOARD_SIZE][BOARD_SIZE], Player *player, Boat *boat);

void place_ia_boats(Cell board[BOARD_SIZE][BOARD_SIZE], Player *player);

void unload_player(Player *player);

void player_play(WINDOW *win, Cell board[BOARD_SIZE][BOARD_SIZE], Player *human, Player *ia, int turn);

void ia_play(WINDOW *win, Cell board[BOARD_SIZE][BOARD_SIZE], Player *human, Player *ia, int turn);

#endif
