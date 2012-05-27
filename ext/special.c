/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2012, Enrico Pilotto <enrico@megiston.it>
 * This code is under LICENSE LGPLv3
 */

#include "special.h"

void
update_castling (Board *board, int from)
{
  switch (from)
    {
    case A1:
      board->castling ^= board->castling & 0x0100;
      break;
    case E1:
      board->castling ^= board->castling & 0x1100;
      break;
    case H1:
      board->castling ^= board->castling & 0x1000;
      break;
    case A8:
      board->castling ^= board->castling & 0x0001;
      break;
    case E8:
      board->castling ^= board->castling & 0x0011;
      break;
    case H8:
      board->castling ^= board->castling & 0x0010;
      break;
    }
}

int
castling_type (Board *board, int from, int to)
{
  if (from == E1 && to == G1)
    {
      if (!(board->castling & 0x1000)
        || (board->occupied & 0x60)
        || capture (board, BLACK, E1)
        || capture (board, BLACK, F1)
        || capture (board, BLACK, G1))
        return 0;
      return WHITE_SHORT_CASTLING;
    }
  if (from == E1 && to == C1)
    {
      if (!(board->castling & 0x0100)
        || (board->occupied & 0x0E)
        || capture (board, BLACK, E1)
        || capture (board, BLACK, D1)
        || capture (board, BLACK, C1))
        return 0;
      return WHITE_LONG_CASTLING;
    }
  if (from == E8 && to == G8)
    {
      if (!(board->castling & 0x0010)
        || (board->occupied & 0x6000000000000000)
        || capture (board, WHITE, E8)
        || capture (board, WHITE, F8)
        || capture (board, WHITE, G8))
        return 0;
      return BLACK_SHORT_CASTLING;
    }
  if (from == E8 && to == C8)
    {
      if (!(board->castling & 0x0001)
        || (board->occupied & 0x0E00000000000000)
        || capture (board, WHITE, E8)
        || capture (board, WHITE, D8)
        || capture (board, WHITE, C8))
        return 0;
      return BLACK_LONG_CASTLING;
    }
  return 0;
}

char*
castling (Board *board, int castling_type, Board *new_board)
{
  char *move;
  switch (castling_type)
    {
    case WHITE_SHORT_CASTLING:
      memcpy (new_board, board, sizeof (Board));
      new_board->king[WHITE] ^= 0x50;
      new_board->rooks[WHITE] ^= 0xa0;
      new_board->placement[E1] = 0;
      new_board->placement[F1] = 'R';
      new_board->placement[G1] = 'K';
      new_board->placement[H1] = 0;
      new_board->castling ^= new_board->castling & 0x1100;
      move = (char *) malloc (5);
      strcpy (move, "O-O");
      break;
    case WHITE_LONG_CASTLING:
      memcpy (new_board, board, sizeof (Board));
      new_board->king[WHITE] ^= 0x14;
      new_board->rooks[WHITE] ^= 0x09;
      new_board->placement[E1] = 0;
      new_board->placement[D1] = 'R';
      new_board->placement[C1] = 'K';
      new_board->placement[A1] = 0;
      new_board->castling ^= new_board->castling & 0x1100;
      move = (char *) malloc (7);
      strcpy (move, "O-O-O");
      break;
    case BLACK_SHORT_CASTLING:
      memcpy (new_board, board, sizeof (Board));
      new_board->king[BLACK] ^= 0x5000000000000000;
      new_board->rooks[BLACK] ^= 0xa000000000000000;
      new_board->placement[E8] = 0;
      new_board->placement[F8] = 'r';
      new_board->placement[G8] = 'k';
      new_board->placement[H8] = 0;
      new_board->castling ^= new_board->castling & 0x0011;
      move = (char *) malloc (5);
      strcpy (move, "O-O");
      break;
    case BLACK_LONG_CASTLING:
      memcpy (new_board, board, sizeof (Board));
      new_board->king[BLACK] ^= 0x1400000000000000;
      new_board->rooks[BLACK] ^= 0x0900000000000000;
      new_board->placement[E8] = 0;
      new_board->placement[D8] = 'r';
      new_board->placement[C8] = 'k';
      new_board->placement[A8] = 0;
      new_board->castling ^= new_board->castling & 0x0011;
      move = (char *) malloc (7);
      strcpy (move, "O-O-O");
      break;
    default:
      return 0;
    }
  set_occupied (new_board);
  return move;
}

void
update_en_passant (Board *board, int from, int to)
{
  if (board->placement[to] == 'P' && from >= A2 && from <= H2 && to == from + 16)
    board->en_passant = to - 8;
  else if (board->placement[to] == 'p' && from >= A7 && from <= H7 && to == from - 16)
    board->en_passant = to + 8;
  else
    board->en_passant = -1;
}

int
have_en_passant (Board *board, int from, int to)
{
  if (board->placement[from] == 'P' && to == board->en_passant
      && (from == to - 7 || from == to - 9))
    return to - 8;
  else if (board->placement[from] == 'p' && to == board->en_passant
      && (from == to + 7 || from == to + 9))
    return to + 8;
  return 0;
}

int
have_en_passant2 (Board *board, int to)
{
  return have_en_passant (board, to + 1, board->en_passant) || have_en_passant (board, to - 1, board->en_passant);
}

bool
require_a_promotion (Board *board)
{
  return ((board->pawns[WHITE] | board->pawns[BLACK]) & 0xff000000000000ff) ? TRUE : FALSE;
}

bool
promote (Board *board, int square, char promote_in)
{
  if (board->active_color)
    promote_in = tolower (promote_in);
  else
    promote_in = toupper (promote_in);
  *(get_bitboard (board, square)) ^= 1ULL << square;
  bboard *bb = get_piece_bitboard (board, promote_in);
  if (!bb) return FALSE;
  *(bb) ^= 1ULL << square;
  board->placement[square] = promote_in;
  return TRUE;
}
