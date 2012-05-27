/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2012, Enrico Pilotto <enrico@megiston.it>
 * This code is under LICENSE LGPLv3
 */

#include "game.h"

static Board STARTING_BOARD;

void
init_chess_library ()
{
  init_board (&STARTING_BOARD);
  precalculate_all_xray ();
}

Game*
init_game ()
{
  Game *g = NEW_GAME;
  g->current = 0;
  g->result = IN_PROGRESS;
  return g;
}

// Free memory of a game
void
free_game (Game *g)
{
  int i;
  for (i = 0; i < g->current; i++)
    {
      free (g->boards[i]);
      free (g->moves[i]);
      free (g->full_moves[i]);
    }
  free (g);
}

Board*
current_board (Game *g)
{
  if (g->current > 0)
    return g->boards[g->current-1];
  return &STARTING_BOARD;
}

Board*
get_board (Game *g, int index)
{
  if (index == 0)
    return &STARTING_BOARD;
  if (index < g->current)
    return g->boards[index];
  return 0;
}

char*
current_move (Game *g)
{
  if (g->current > 0)
    return g->moves[g->current-1];
  return 0;
}

char*
current_full_move (Game *g)
{
  if (g->current > 0)
    return g->full_moves[g->current-1];
  return 0;
}

bool
apply_move (Game *g, int from, int to, char promote_in)
{
  if (g->result != IN_PROGRESS) return FALSE;
  Board *board = current_board (g);
  Board *new_board = NEW_BOARD;
  char capture = 0;
  char *move_done = castling (board, castling_type (board, from, to), new_board);
  if (!move_done) // If move is not a castling
    {
      if (!try_move (board, from, to, promote_in, new_board, &move_done, &capture))
        {
          free (new_board);
          return FALSE;
        }
    }
  // Ok move is legal, update the game
  update_castling (new_board, from);
  update_en_passant (new_board, from, to);
  if (new_board->active_color)
    new_board->fullmove_number++;
  new_board->active_color = !new_board->active_color;
  if (capture || toupper (board->placement[from]) == 'P')
    new_board->halfmove_clock = 0;
  else
    new_board->halfmove_clock++;
  g->boards[g->current] = new_board;
  g->moves[g->current] = move_done;
  g->full_moves[g->current] = ft_to_full_move (from, to);
  g->current++;
  // Test check or checkmate of opponent king
  if (king_in_check (new_board, new_board->active_color))
    {
      if (king_in_checkmate (new_board, new_board->active_color))
        {
          //printf ("WE HAVE A CHECKMATE, %d win!!!\n", !new_board->active_color);
          //printf ("%s\n", print_board (new_board));
          strcat (move_done, "#");
          g->result = !new_board->active_color;
        }
      else
        strcat (move_done, "+");
    }
  return TRUE;
}

void
rollback (Game *g)
{
  if (g->current > 0)
    {
      g->current--;
      free (g->boards[g->current]);
      free (g->moves[g->current]);
      free (g->full_moves[g->current]);
    }
}

//////////
// MAIN //
//////////
int
main ()
{
  // Valgrind run
  init_chess_library ();
  int i, from, to;

  Game *ga = init_game ();
  Board *board = current_board (ga);
  bboard king = board->king[WHITE];

  print_bitboard (king);
  printf ("%d\n", square2 (king));
  print_bitboard (xray (board, 0, FALSE) & ~board->pieces[WHITE]);
  return 0;
  for (i = 0; i < 1000; i++)
  {
    Game *g = init_game ();
    Board *board = current_board (g);
    get_coord (board, 'P', "e", "e4", 0, &from, &to);
    pseudo_legal_move (board, from, to);
    apply_move (g, from, to, 0);
    char *fen = to_fen (current_board (g));
    free_game (g);
    free (fen);
  }

  /*
  init_chess_library ();

  Game *g = init_game ();

  int i, from, to;
  char string[6], promo;

  while (1)
    {
      printf ("\n-----------------\n");
      printf ("%s\n", print_board (current_board (g)));
      printf ("Give me a move: ");
      fgets (string, 7, stdin);
      i = (int) strlen (string) - 1;
      string[i] = '\0';
      from = (string[0] - 97) + ((string[1] - 49) * 8);
      to = (string[2] - 97) + ((string[3] - 49) * 8);
      promo = string[4];
      if (pseudo_legal_move (s, from, to) && apply_move (s, from, to, promo))
          printf ("Move done is '%s'\n", current_move (g));
      else
        printf ("Invalid move\n");

    }
  free_game (g);
  */
}
