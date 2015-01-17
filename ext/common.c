/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2012, Enrico Pilotto <enrico@megiston.it>
 * This code is under LICENSE LGPLv3
 */

#include "common.h"

char
square_to_file (int square)
{
  return (square % 8) + 97;
}

char
square_to_rank (int square)
{
  return (square / 8) + 49;
}

int
coord_to_square (const char *coord)
{
  return 8 * (coord[1] - 49) + (coord[0] - 97);
}

char*
square_to_coord (int square)
{
  char *s = (char *) malloc (3);
  s[0] = square_to_file (square);
  s[1] = square_to_rank (square);
  s[2] = '\0';
  return s;
}

char*
ft_to_full_move (int from, int to, char promote_in)
{
  char *s = (char *) malloc (7);
  s[0] = square_to_file (from);
  s[1] = square_to_rank (from);
  s[2] = square_to_file (to);
  s[3] = square_to_rank (to);
  s[4] = '\0';
  if (promote_in)
    {
      s[4] = '=';
      s[5] = promote_in;
      s[6] = '\0';
    }
  return s;
}

char*
result_to_s (unsigned short int r)
{
  char *s = (char *) malloc (8);
  switch (r)
    {
    case WHITE_WON:
      strcpy (s, "1-0");
      break;
    case BLACK_WON:
      strcpy (s, "0-1");
      break;
    case DRAW:
      strcpy (s, "1/2-1/2");
      break;
    default:
      strcpy (s, "*");
      break;
    }
  return s;
}

char*
castling_to_s (short int castling)
{
  char *s = (char *) malloc (5);
  int cur = 0;
  if (0x1000 & castling) { s[cur] = 'K'; cur++; }
  if (0x0100 & castling) { s[cur] = 'Q'; cur++; }
  if (0x0010 & castling) { s[cur] = 'k'; cur++; }
  if (0x0001 & castling) { s[cur] = 'q'; cur++; }
  if (cur == 0) { s[cur] = '-'; cur++; }
  s[cur] = '\0';
  return s;
}

char*
en_passant_to_s (short int en_passant)
{
  char *s = (char *) malloc (3);
  if (en_passant == -1)
    strcpy (s, "-");
  else
    strcpy (s, square_to_coord (en_passant));
  return s;
}

int
compare (const void *a, const void *b)
{
  return strcmp (*(const char **) a, *(const char **) b);
}
