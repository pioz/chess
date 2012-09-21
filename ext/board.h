/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2012, Enrico Pilotto <enrico@megiston.it>
 * This code is under LICENSE LGPLv3
 */

#ifndef BOARD_H
#define BOARD_H

#include "bitboard.h"

typedef struct Board
{
  char placement[64];
  short int active_color;
  short int castling;
  short int en_passant;
  unsigned int halfmove_clock;
  unsigned int fullmove_number;
  // Pieces bitboards - 0 means white 1 means black
  bboard pawns[2];
  bboard rooks[2];
  bboard knights[2];
  bboard bishops[2];
  bboard queens[2];
  bboard king[2];
  // Userful bitboards
  bboard pieces[2];
  bboard occupied;
} Board;

#define NEW_BOARD (Board*) malloc (sizeof (Board))

#include "common.h"
#include "special.h"

void init_board (Board *board);
void set_occupied (Board *board);
char* print_board (Board *board);
int get_color (Board *board, int square);
bboard* get_piece_bitboard (Board *board, char piece);
bboard* get_bitboard (Board *board, int square);
bboard xray (Board *board, int from, bool only_attack);
bboard all_xray (Board *board, int color, bool only_attack);
bboard all_xray_friend (Board *board, int color, bool only_attack);
void remove_piece (Board *board, int square, Board *new_board);
int same_pieces_that_can_capture_a_square (Board *board, int color, int square, int *pieces, char piece_filter);
bool capture (Board *board, int color, int square);
bool pieces_can_safe_capture (Board *board, int color, int square);
bool king_in_check (Board *board, int color);
bool king_in_checkmate (Board *board, int color);
bool stalemate (Board *board, int color);
bool insufficient_material (Board *board);
bool fifty_move_rule (Board *board);
bool pseudo_legal_move (Board *board, int from, int to);
void get_coord (Board *board, char piece, const char* disambiguating, const char *to_coord, char promote_in, int *from, int *to);
bool try_move (Board *board, int from, int to, char promote_in, Board *new_board, char **move_done, char *capture);
char* get_notation (Board *board, int from, int to, int capture, int ep, char promotion, int check, int checkmate);
char* to_fen (Board *board);

#endif
