#include <stdlib.h>
#include "player.h"

void player_factory(Player *player, Player_type type){
	switch (type) {
		case IA:
			new_player(player, 0, ia_play);
			break;
		case HUMAN:
			new_player(player, 0, player_play);
			break;
		default:
			player = NULL;
			break;
	}
}

void new_player (Player *player, int boats_alive, void (*play)(WINDOW *win, Cell board[BOARD_SIZE][BOARD_SIZE], Player *human, Player*ia, int turn)){
	int i;
	Boat_type boats[5] = {CARRIER, BATTLESHIP, CRUISER, SUBMARINE, DESTROYER};

	player->boats_alive = boats_alive;
	player->play = play;

	for (i = 0; i < 5; i++) {
		player->boats[i] = malloc(sizeof(Boat));
		boat_factory(player->boats[i], boats[i]);
	}
	for (i = 0; i < 100; i++) {
		strcpy(player->history[i], "--");
	}
}

void hit (Cell *cell, Player *hit_player) {
	hit_cell(cell);
	
	if (cell->boat != NULL) {
		cell->boat->hits++;

		if (cell->boat->size == cell->boat->hits) { /* If boat destroyed  */
			hit_player->boats_alive--;	
		}
	}
}

void ask_player_to_place_boats(WINDOW *win, Cell board[BOARD_SIZE][BOARD_SIZE], Player *player) {		
	int i;

	for (i = 0; i < 5; i++) {
		ask_player_to_place_one_boat(win, board, player, player->boats[i]);
	}
}


void unload_player(Player *player) {
	int i;
	
	for (i =  0; i < 5; i++) {
		free(player->boats[i]);
	}
}

void ask_player_to_place_one_boat(WINDOW *win, Cell board[BOARD_SIZE][BOARD_SIZE], Player *player, Boat *boat) {
	Direction direction;
	int row, col;
	char *starting_position;

	do {
		starting_position = ask_boat_position(win, board, *boat, &direction);

		row = starting_position[0] - 'A';
		col = starting_position[1] - '0';
	
		free(starting_position);
	} while (check_if_boat_feets_in_board(board, *boat, row, col, direction) == 0);
		
	place_boat(board, boat, row, col, direction);
	boat->start_row = row;
	boat->start_col = col;
	boat->direction = direction;
	player->boats_alive++;
}

void place_ia_boats(Cell board[BOARD_SIZE][BOARD_SIZE], Player *player) {
	int i, row, col, dir;
	const int min_row = 0;
	const int min_col = 0;
	const int max_row = 9;
	const int max_col = 9;
	Direction dirs[4] = {NORTH, EAST, SOUTH, WEST};

	srand(time(NULL));

	for (i = 0; i < 5; i++) {
		do {
			row = min_row + rand() % (max_row + 1 - min_row);
			col = min_col + rand() % (max_col + 1 - min_col);
			dir = 0 + rand() % (3 + 1 - 0);
		} while (check_if_boat_feets_in_board(board, *player->boats[i], row, col, dirs[dir]) == 0);
		
		place_boat(board, player->boats[i], row, col, dirs[dir]);
		player->boats[i]->start_row = row;
		player->boats[i]->start_col = col;
		player->boats[i]->direction = dirs[dir];
		player->boats_alive++;
	}
}

void player_play (WINDOW *win, Cell board[BOARD_SIZE][BOARD_SIZE], Player *human, Player *ia, int turn) {
	char *coords;
	int row, col;
	char previous_shot[3] = "A0";
	
	if (turn > 0) {
		strcpy(previous_shot, human->history[turn-1]);
	}

	do {
		coords = get_coords(board, win, previous_shot[0] - 'A', previous_shot[1] - '0');
		
		row = coords[0] - 'A';
		col = coords[1] - '0';

		if (check_shot_possible(board, row, col) == 0) {
			free(coords);
		}
	} while (check_shot_possible(board, row, col) == 0);

	hit(&board[row][col], ia);
	strcpy(human->history[turn], coords); 
	free(coords);
}

void ia_play (WINDOW *win, Cell board[BOARD_SIZE][BOARD_SIZE], Player *human, Player *ia, int turn) {
	int row, col;
	const int min_row = 0;
	const int min_col = 0;
	const int max_row = 9;
	const int max_col = 9;
	char coords[2];
	srand(time(NULL));
		
	do {
		row = min_row + rand() % (max_row + 1 - min_row);
		col = min_col + rand() % (max_col + 1 - min_col);
	} while (check_shot_possible(board, row, col) == 0);

	coords[0] = row + 'A';
	coords[1] = col + '0';

	hit(&board[row][col], human);
	strcpy(ia->history[turn], coords);
	wrefresh(win);
}
