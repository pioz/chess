/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2017, Enrico Pilotto <epilotto@gmx.com>
 * This code is under LICENSE LGPLv3
 */

#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdio.h>
#include <stdint.h>

// Types
typedef uint64_t bboard;

// Useful bitboards definitions
#define EMPTY_BOARD   0x0000000000000000
#define FULL_BOARD    0xffffffffffffffff
#define NOT_FILE_A    0xfefefefefefefefe
#define NOT_FILE_AB   0xfcfcfcfcfcfcfcfc
#define NOT_FILE_H    0x7f7f7f7f7f7f7f7f
#define NOT_FILE_GH   0x3f3f3f3f3f3f3f3f
#define WHITE_SQUARES 0x55aa55aa55aa55aa
#define BLACK_SQUARES 0xaa55aa55aa55aa55

// Bitboard getters
int square (int file, int rank);
int square2 (bboard b);
void squares (bboard b, int *array, int *n);
int file (int square);
int rank (int square);
bboard get (bboard b, int square);
bboard get2 (bboard b, int file, int rank);
void print_bitboard (bboard b);
// Bitboard manipulations
bboard has_only_one_one (bboard b);
bboard only_white_squares (bboard b);
bboard only_black_squares (bboard b);
bboard mirror_horizontal (bboard b);
bboard mirror_vertical (bboard b);
bboard rotate_left (bboard b, int s);
bboard rotate_right (bboard b, int s);
bboard occluded_fill (bboard gen, bboard pro, int dir);
bboard shift_one (bboard b, int dir);
bboard sliding_attacks (bboard slider, bboard propagator, int dir);
// Pre-calculators
void precalculate_xray_attack_white_pawn (bboard xray[64]);
void precalculate_xray_attack_black_pawn (bboard xray[64]);
void precalculate_xray_knight (bboard xray[64]);
void precalculate_xray_king (bboard xray[64]);
void precalculate_all_xray ();
// XRay generators
bboard xray_white_pawn (bboard occupied_square, int square);
bboard xray_black_pawn (bboard occupied_square, int square);
bboard xray_attack_white_pawn (int square);
bboard xray_attack_black_pawn (int square);
bboard xray_rook (bboard occupied_square, int square);
bboard xray_knight (int square);
bboard xray_bishop (bboard occupied_square, int square);
bboard xray_queen (bboard occupied_square, int square);
bboard xray_king (int square);

#endif
