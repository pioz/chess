/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2018, Enrico Pilotto <epilotto@gmx.com>
 * This code is under LICENSE LGPLv3
 */

#ifndef GAME_H
#define GAME_H

#define NEW_GAME (Game*) malloc (sizeof (Game))

#define BUFFER_SIZE 1024

#include "common.h"
#include "board.h"

typedef struct
{
  Board* boards[BUFFER_SIZE];
  char* moves[BUFFER_SIZE];
  char* coord_moves[BUFFER_SIZE];
  int current;
  unsigned short result;
} Game;


#include "special.h"

void init_chess_library ();
Game* init_game ();
void free_game (Game *g);
Board* current_board (Game *g);
Board* get_board (Game *g, int index);
char* current_move (Game *g);
char* current_coord_move (Game *g);
bool apply_move (Game *g, int from, int to, char promote_in);
void rollback (Game *g);
bool threefold_repetition (Game *g);
void set_fen (Game *g, const char *fen);

#endif
