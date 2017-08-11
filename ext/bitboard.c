/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2017, Enrico Pilotto <epilotto@gmx.com>
 * This code is under LICENSE LGPLv3
 */

#include "bitboard.h"

// Pre-calculate variables
bboard ALL_XRAY_ATTACK_WHITE_PAWN[64];
bboard ALL_XRAY_ATTACK_BLACK_PAWN[64];
bboard ALL_XRAY_KNIGHT[64];
bboard ALL_XRAY_KING[64];

/*
  Shift by direction
   [7]      [0]      [1]
       +7   +8    +9
          \  |  /
   [6] -1 <- X -> +1 [2]
          /  |  \
       -9   -8    -7
   [5]      [4]      [3]
 */
const int DIR[8] = { 8, 9, 1, -7, -8, -9, -1, 7 };

/*
  Avoid wrap bitboard masks
   [7] -> +7           [0] -> +8           [1] -> +9
   0x7F7F7F7F7F7F7F00  0xFFFFFFFFFFFFFF00  0xFEFEFEFEFEFEFE00
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   . . . . . . . .     . . . . . . . .     . . . . . . . .

   [6] -> -1                               [2] -> +1
   0x7F7F7F7F7F7F7F7F                      0xFEFEFEFEFEFEFEFE
   1 1 1 1 1 1 1 .                         . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .                         . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .                         . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .                         . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .                         . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .                         . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .                         . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .                         . 1 1 1 1 1 1 1

   [5] -> -9           [4] -> -8           [3] -> -7
   0x007F7F7F7F7F7F7F  0x00FFFFFFFFFFFFFF  0x00FEFEFEFEFEFEFE
   . . . . . . . .     . . . . . . . .     . . . . . . . .
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
   1 1 1 1 1 1 1 .     1 1 1 1 1 1 1 1     . 1 1 1 1 1 1 1
 */
const bboard AVOID_WRAP[8] =
{
  0xffffffffffffff00,
  0xfefefefefefefe00,
  0xfefefefefefefefe,
  0x00fefefefefefefe,
  0x00ffffffffffffff,
  0x007f7f7f7f7f7f7f,
  0x7f7f7f7f7f7f7f7f,
  0x7f7f7f7f7f7f7f00
};

// Bitboard getters

// Given file and rank returns the square (0..63).
int
square (int file, int rank)
{
  return 8 * rank + file;
}

// Given a bitboard returns the first square (0..63) with a piece.
int
square2 (bboard b)
{
  int pos = -1;
  while (b)
    {
      b >>= 1;
      pos++;
    }
  return pos;
}

// Returns the array and its size (n) with pieces in the bitboard.
void
squares (bboard b, int *array, int *n)
{
  *n = 0;
  for (int i = 0; i < 64; i++)
    if (b & 1ULL << i)
      {
        array[*n] = i;
        (*n)++;
      }
}

// Given a square returns the corresponding file.
int
file (int square)
{
  return square & 7;
}

// Given a square returns the corresponding rank.
int rank (int square)
{
  return square >> 3;
}

// Given a bitboard and a square (0..63) returns a bitboard copy with square set
// to 1.
bboard
get (bboard b, int square)
{
  return b & (1ULL << square);
}

// Given a bitboard, a file and a rank returns a bitboard copy with square set
// to 1.
bboard
get2 (bboard b, int file, int rank)
{
  return b & (1ULL << (8 * rank + file));
}

// Print the bitboard.
void
print_bitboard (bboard b)
{
  for (int i = 7; i >= 0; i--) // rank => top to bottom
    {
      for (int j = 0; j < 8; j++) // file => left to right
        printf ("%d ", get2 (b, j, i) ? 1 : 0);
      printf ("\n");
    }
}

// Bitboard manipulations

// Returns true if the bitboard has only one bit equals to 1.
bboard
has_only_one_one (bboard b)
{
  return b && !(b & (b-1));
}

// Returns true if the bitboard has only bits equals to 1 in the white squares.
bboard
only_white_squares (bboard b)
{
  return !((b ^ WHITE_SQUARES) & b);
}

// Returns true if the bitboard has only bits equals to 1 in the black squares.
bboard
only_black_squares (bboard b)
{
  return !((b ^ BLACK_SQUARES) & b);
}

// Returns the horizontally mirrored bitboard.
bboard
mirror_horizontal (bboard b)
{
  const bboard k1 = 0x5555555555555555;
  const bboard k2 = 0x3333333333333333;
  const bboard k3 = 0x0f0f0f0f0f0f0f0f;
  b = ((b >> 1) & k1) +  2 * (b & k1);
  b = ((b >> 2) & k2) +  4 * (b & k2);
  b = ((b >> 4) & k3) + 16 * (b & k3);
  return b;
}

// Returns the vertically mirrored bitboard.
bboard
mirror_vertical (bboard b)
{
  const bboard k1 = 0x00ff00ff00ff00ff;
  const bboard k2 = 0x0000ffff0000ffff;
  b = ((b >>  8) & k1) | ((b & k1) <<  8);
  b = ((b >> 16) & k2) | ((b & k2) << 16);
  b = ( b >> 32)       | ( b       << 32);
  return b;
}

// Returns the left rotated bitboard.
bboard
rotate_left (bboard b, int s)
{
  //return (b << s) | (b >> (64 - s));
  //return (b << s) | (b >> -s);
  return s > 0 ? b << s : b >> -s;
}

// Returns the right rotated bitboard.
bboard
rotate_right (bboard b, int s)
{
  //return (b >> s) | (b << (64 - s));
  //return (b >> s) | (b << -s);
  return s > 0 ? b >> s : b << -s;
}

// Given a generator, a propagator and a direction returns the bitboard with
// occluded squares setted to 1.
bboard
occluded_fill (bboard gen, bboard pro, int dir)
{
   int r = DIR[dir];
   pro &= AVOID_WRAP[dir];
   gen |= pro & rotate_left (gen, r);
   pro &=       rotate_left (pro, r);
   gen |= pro & rotate_left (gen, 2 * r);
   pro &=       rotate_left (pro, 2 * r);
   gen |= pro & rotate_left (gen, 4 * r);
   return gen;
}

// Returns the shifted bitboard.
bboard
shift_one (bboard b, int dir)
{
  return rotate_left (b, DIR[dir]) & AVOID_WRAP[dir];
}

// Returns the bitboard with sliding attacks.
bboard
sliding_attacks (bboard slider, bboard propagator, int dir)
{
  bboard fill = occluded_fill (slider, propagator, dir);
  return shift_one (fill, dir);
}

// Pre-calculators

void
precalculate_xray_attack_white_pawn (bboard xray[64])
{
  for (int i = 0; i < 64; i++)
    {
      xray[i] = EMPTY_BOARD;
      xray[i] = shift_one (1ULL << i, 7) | shift_one (1ULL << i, 1);
    }
}

void
precalculate_xray_attack_black_pawn (bboard xray[64])
{
  for (int i = 0; i < 64; i++)
    {
      xray[i] = EMPTY_BOARD;
      xray[i] = shift_one (1ULL << i, 5) | shift_one (1ULL << i, 3);
    }
}

void
precalculate_xray_knight (bboard xray[64])
{
  for (int i = 0; i < 64; i++)
    {
      xray[i] = 1ULL << i;
      xray[i] = ((xray[i] << 17) & NOT_FILE_A)
        | ((xray[i] << 10) & NOT_FILE_AB)
        | ((xray[i] >>  6) & NOT_FILE_AB)
        | ((xray[i] >> 15) & NOT_FILE_A)
        | ((xray[i] << 15) & NOT_FILE_H)
        | ((xray[i] <<  6) & NOT_FILE_GH)
        | ((xray[i] >> 10) & NOT_FILE_GH)
        | ((xray[i] >> 17) & NOT_FILE_H);
    }
}

void
precalculate_xray_king (bboard xray[64])
{
  for (int i = 0; i < 64; i++)
    {
      xray[i] = EMPTY_BOARD;
      for (int j = 0; j < 8; j++)
        xray[i] = sliding_attacks (1ULL << i, EMPTY_BOARD, j) | xray[i];
    }
}

void
precalculate_all_xray ()
{
  precalculate_xray_attack_white_pawn (ALL_XRAY_ATTACK_WHITE_PAWN);
  precalculate_xray_attack_black_pawn (ALL_XRAY_ATTACK_BLACK_PAWN);
  precalculate_xray_knight (ALL_XRAY_KNIGHT);
  precalculate_xray_king (ALL_XRAY_KING);
}

// XRay generators

bboard
xray_white_pawn (bboard occupied_square, int square)
{
  bboard empty_square = ~occupied_square;
  bboard one_step = shift_one (1ULL << square, 0) & empty_square;
  if (square > 7 && square < 16)
    {
      bboard two_step = shift_one (1ULL << (square + 8), 0) & empty_square & ~shift_one (occupied_square, 0);
      return one_step | two_step | (ALL_XRAY_ATTACK_WHITE_PAWN[square] & occupied_square);
    }
  return one_step | (ALL_XRAY_ATTACK_WHITE_PAWN[square] & occupied_square);
}

bboard
xray_black_pawn (bboard occupied_square, int square)
{
  bboard empty_square = ~occupied_square;
  bboard one_step = shift_one (1ULL << square, 4) & empty_square;
  if (square > 47 && square < 56)
    {
      bboard two_step = shift_one (1ULL << (square - 8), 4) & empty_square & ~shift_one (occupied_square, 4);
      return one_step | two_step | (ALL_XRAY_ATTACK_BLACK_PAWN[square] & occupied_square);
    }
  return one_step | (ALL_XRAY_ATTACK_BLACK_PAWN[square] & occupied_square);
}

bboard
xray_attack_white_pawn (int square)
{
  return ALL_XRAY_ATTACK_WHITE_PAWN[square];
}

bboard
xray_attack_black_pawn (int square)
{
  return ALL_XRAY_ATTACK_BLACK_PAWN[square];
}

bboard
xray_rook (bboard occupied_square, int square)
{
  bboard xray[4];
  bboard collision, shielded_square;
  for (int i = 0, dir = 0; i < 4; i++, dir+=2)
    {
      xray[i] = sliding_attacks (1ULL << square, FULL_BOARD, dir);
      collision = xray[i] & occupied_square;
      shielded_square = sliding_attacks (collision, FULL_BOARD, dir);
      xray[i] ^= shielded_square;
    }
  return xray[0] | xray[1] | xray[2] | xray[3];
}

bboard
xray_knight (int square)
{
  return ALL_XRAY_KNIGHT[square];
}

bboard
xray_bishop (bboard occupied_square, int square)
{
  bboard xray[4];
  bboard collision, shielded_square;
  for (int i = 0, dir = 1; i < 4; i++, dir+=2)
    {
      xray[i] = sliding_attacks (1ULL << square, FULL_BOARD, dir);
      collision = xray[i] & occupied_square;
      shielded_square = sliding_attacks (collision, FULL_BOARD, dir);
      xray[i] ^= shielded_square;
    }
  return xray[0] | xray[1] | xray[2] | xray[3];
}

bboard
xray_queen (bboard occupied_square, int square)
{
  return xray_rook (occupied_square, square) | xray_bishop (occupied_square, square);
}

bboard
xray_king (int square)
{
  return ALL_XRAY_KING[square];
}
