/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2012, Enrico Pilotto <enrico@megiston.it>
 * This code is under LICENSE LGPLv3
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Macros
#define FALSE 0
#define TRUE 1

#define A1 0
#define B1 1
#define C1 2
#define D1 3
#define E1 4
#define F1 5
#define G1 6
#define H1 7
#define A2 8
#define B2 9
#define C2 10
#define D2 11
#define E2 12
#define F2 13
#define G2 14
#define H2 15
#define A3 16
#define B3 17
#define C3 18
#define D3 19
#define E3 20
#define F3 21
#define G3 22
#define H3 23
#define A4 24
#define B4 25
#define C4 26
#define D4 27
#define E4 28
#define F4 29
#define G4 30
#define H4 31
#define A5 32
#define B5 33
#define C5 34
#define D5 35
#define E5 36
#define F5 37
#define G5 38
#define H5 39
#define A6 40
#define B6 41
#define C6 42
#define D6 43
#define E6 44
#define F6 45
#define G6 46
#define H6 47
#define A7 48
#define B7 49
#define C7 50
#define D7 51
#define E7 52
#define F7 53
#define G7 54
#define H7 55
#define A8 56
#define B8 57
#define C8 58
#define D8 59
#define E8 60
#define F8 61
#define G8 62
#define H8 63

#define WHITE 0
#define BLACK 1

#define WHITE_SHORT_CASTLING 1
#define WHITE_LONG_CASTLING 2
#define BLACK_SHORT_CASTLING 3
#define BLACK_LONG_CASTLING 4

#define WHITE_WINS 0
#define BLACK_WINS 1
#define DRAW 2
#define IN_PROGRESS 3

typedef int bool;

char square_to_file (int square);
char square_to_rank (int square);
int coord_to_square (const char *coord);
char* square_to_coord (int square);
char* ft_to_full_move (int from, int to, char promote_in);
char* result_to_s (unsigned short int r);
char* castling_to_s (short int castling);
char* en_passant_to_s (short int en_passant);
bool compare (const void * a, const void * b);

#endif