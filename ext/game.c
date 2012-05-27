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

  for (i = 0; i < 1000; i++)
  {
    Game *g = init_game ();
    Board *board;
    char *fen;
    // 1. e4 a6 2. Bc4 a5 3. Qh5 a4 4. Qxf7#
    board = current_board (g);
    get_coord (board, 'P', "e", "e4", 0, &from, &to);
    pseudo_legal_move (board, from, to);
    apply_move (g, from, to, 0);
    fen = to_fen (board);
    free (fen);

    board = current_board (g);
    get_coord (board, 'P', "a", "a6", 0, &from, &to);
    pseudo_legal_move (board, from, to);
    apply_move (g, from, to, 0);
    fen = to_fen (current_board (g));
    free (fen);

    board = current_board (g);
    get_coord (board, 'B', "", "c4", 0, &from, &to);
    pseudo_legal_move (board, from, to);
    apply_move (g, from, to, 0);
    fen = to_fen (current_board (g));
    free (fen);

    board = current_board (g);
    get_coord (board, 'P', "a", "a5", 0, &from, &to);
    pseudo_legal_move (board, from, to);
    apply_move (g, from, to, 0);
    fen = to_fen (current_board (g));
    free (fen);

    board = current_board (g);
    get_coord (board, 'Q', "", "h5", 0, &from, &to);
    pseudo_legal_move (board, from, to);
    apply_move (g, from, to, 0);
    fen = to_fen (current_board (g));
    free (fen);

    board = current_board (g);
    get_coord (board, 'P', "a", "a4", 0, &from, &to);
    pseudo_legal_move (board, from, to);
    apply_move (g, from, to, 0);
    fen = to_fen (current_board (g));
    free (fen);

    board = current_board (g);
    get_coord (board, 'Q', "", "f7", 0, &from, &to);
    pseudo_legal_move (board, from, to);
    apply_move (g, from, to, 0);
    fen = to_fen (current_board (g));
    free (fen);

    free_game (g);
  }
}
