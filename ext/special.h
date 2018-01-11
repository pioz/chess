/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2018, Enrico Pilotto <epilotto@gmx.com>
 * This code is under LICENSE LGPLv3
 */

#ifndef SPECIAL_H
#define SPECIAL_H

#include "common.h"
#include "board.h"

void update_castling_by_square (Board *board, int square);
void update_castling (Board *board, int from, int to);
int castling_type (Board *board, int from, int to);
char* castling (Board *board, int castling_type, Board *new_board);
void update_en_passant (Board *board, int from, int to);
int have_en_passant (Board *board, int from, int to);
bool require_a_promotion (Board *board);
void promote (Board *board, int square, char promote_in);

#endif
