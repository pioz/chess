/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2012, Enrico Pilotto <enrico@megiston.it>
 * This code is under LICENSE LGPLv3
 */

#include "chess.h"

VALUE illegal_move_error;
VALUE board_klass;

// Game

/*
 * Alloc function
 */
VALUE
game_alloc (VALUE class)
{
  Game *g = init_game (g);
  return Data_Wrap_Struct (class, 0, free_game, g);
  // VALUE tdata = Data_Wrap_Struct (class, 0, free_game, g);
  // rb_obj_call_init (tdata, 0, 0);
  // return tdata;
}

/*
 * call-seq: move(piece, disambiguating, to_coord, promote_in)
 *
 * Make a move. This add a new Board in the Game.
 *
 * Parameters are:
 * +piece+:: the character of the moving piece <em>('P', 'R', 'N', 'B', 'Q', 'K')</em>.
 * +disambiguating+:: when two (or more) identical pieces can move to the same square, the moving piece is uniquely identified by specifying the piece's letter, followed by (in descending order of preference):
 *                    1. the file of departure (if they differ); or
 *                    2. the rank of departure (if the files are the same but the ranks differ); or
 *                    3. both the rank and file (if neither alone is sufficient to identify the piece—which occurs only in rare cases where one or more pawns have promoted, resulting in a player having three or more identical pieces able to reach the same square).
 *                    Keep blank if no needed.
 * +to_coord+:: the square where the moving piece will <em>('a1', 'a2', ... , 'h7', 'h8')</em>.
 * +promote_in+:: the character of promotion piece <em>('R', 'N', 'B', 'Q')</em>. If no promotion occured, this param will be ignored.
 * This method returns a string that represents the short algebraic chess notation of the move or raise an IllegalMoveError if the move is illegal.
 */
VALUE
game_move (VALUE self, VALUE rb_piece, VALUE rb_disambiguating, VALUE rb_to_coord, VALUE rb_promote_in)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  Board *board = current_board (g);
  char piece = StringValuePtr (rb_piece)[0];
  char *disambiguating = StringValuePtr (rb_disambiguating);
  char *to_coord = StringValuePtr (rb_to_coord);
  char promote_in = StringValuePtr (rb_promote_in)[0];
  int from, to;
  get_coord (board, piece, disambiguating, to_coord, promote_in, &from, &to);
  // printf ("From: %d, To: %d, Promo: %c\n", from , to , promote_in);
  if (pseudo_legal_move (board, from, to) && apply_move (g, from, to, promote_in))
    return rb_str_new2 (current_move (g));
  else
    rb_raise (illegal_move_error, "Illegal move");
}

/*
 * call-seq: move2(from, to, promote_in)
 *
 * Make a move. This add a new Board in the Game.
 *
 * Parameters are:
 * +from+:: The 2 character string representing the starting square of the moving piece <em>('a1', 'a2', ... , 'h7', 'h8')</em>.
 * +to+:: The 2 character string representing the ending square of the moving piece <em>('a1', 'a2', ... , 'h7', 'h8')</em>.
 * +promote_in+:: the character of promotion piece <em>('R', 'N', 'B', 'Q')</em>. If no promotion occured, this param will be ignored.
 * This method returns a string that represents the short algebraic chess notation of the move or raise an IllegalMoveError if the move is illegal.
 */
VALUE
game_move2 (VALUE self, VALUE rb_from, VALUE rb_to, VALUE rb_promote_in)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  Board *board = current_board (g);
  int from = coord_to_square (StringValuePtr (rb_from));
  int to = coord_to_square (StringValuePtr (rb_to));
  char promote_in = StringValuePtr (rb_promote_in)[0];
  if (pseudo_legal_move (board, from, to) && apply_move (g, from, to, promote_in))
    return rb_str_new2 (current_move (g));
  else
    rb_raise (illegal_move_error, "Illegal move");
}

/*
 * call-seq: move3(from, to, promote_in)
 *
 * Make a move. This add a new Board in the Game.
 *
 * Each square on the chessboard is represented by an integer according to the following scheme:
 *    8 | 56 57 58 59 60 61 62 63
 *    7 | 48 49 50 51 52 53 54 55
 *    6 | 40 41 42 43 44 45 46 47
 *    5 | 32 33 34 35 36 37 38 39
 *    4 | 24 25 26 27 28 29 30 31
 *    3 | 16 17 18 19 20 21 22 23
 *    2 |  8  9 10 11 12 13 14 15
 *    1 |  0  1  2  3  4  5  6  7
 *      +-------------------------
 *         a  b  c  d  e  f  g  h
 * Parameters are:
 * +from+:: The integer representing the starting square of the moving piece.
 * +to+:: The integer representing the ending square of the moving piece.
 * +promote_in+:: the character of promotion piece <em>('R', 'N', 'B', 'Q')</em>. If no promotion occured, this param will be ignored.
 * This method returns a string that represents the short algebraic chess notation of the move or raise an IllegalMoveError if the move is illegal.
 */
VALUE
game_move3 (VALUE self, VALUE rb_from, VALUE rb_to, VALUE rb_promote_in)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  Board *board = current_board (g);
  int from = FIX2INT (rb_from);
  int to = FIX2INT (rb_to);
  char promote_in = StringValuePtr (rb_promote_in)[0];
  if (pseudo_legal_move (board, from, to) && apply_move (g, from, to, promote_in))
    return rb_str_new2 (current_move (g));
  else
    rb_raise (illegal_move_error, "Illegal move");
}

/*
 * call-seq: [n]
 *
 * Returns the +n+-th Board of the Game or +nil+ if the +n+-th Board does not exist.
 */
VALUE
game_boards (VALUE self, VALUE index)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  int n = FIX2INT (index);
  Board *board = get_board (g, n);
  if (board)
    return Data_Wrap_Struct (board_klass, 0, 0, board);
  return Qnil;
}

/*
 * call-seq: current
 *
 * Returns the current Board of the Game.
 */
VALUE
game_last_board (VALUE self)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  return game_boards (self, INT2FIX (g->current-1));
}

/*
 * call-seq: moves
 *
 * Returns the array with all moves done <em>(es: Nc3)</em>.
 */
VALUE
game_moves (VALUE self)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  int i;
  VALUE moves = rb_ary_new ();
  for (i = 0; i < g->current; i++)
    rb_ary_push (moves, rb_str_new2 (g->moves[i]));
  return moves;
}

/*
 * call-seq: full_moves
 *
 * Returns the array with all full moves done <em>(es: b1c3)</em>.
 */
VALUE
game_full_moves (VALUE self)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  int i;
  VALUE moves = rb_ary_new ();
  for (i = 0; i < g->current; i++)
    rb_ary_push (moves, rb_str_new2 (g->full_moves[i]));
  return moves;
}

/*
 * call-seq: size
 *
 * Returns the number of moves done.
 */
VALUE
game_size (VALUE self)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  return INT2FIX (g->current);
}

/*
 * call-seq: result
 *
 * Returns the result of the game:
 * *:: game in progress;
 * 1-0:: white wins;
 * 0-1:: black wins;
 * 1/2-1/2:: draw
 */
VALUE
game_result (VALUE self)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  char *result = result_to_s (g->result);
  VALUE rb_result = rb_str_new2 (result);
  free (result);
  return rb_result;
}

// Board

/*
 * call-seq: check?
 *
 * Returns +true+ if the king of the color that has the turn is in check, +false otherwise+.
 */
VALUE
board_king_in_check (VALUE self)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  if (king_in_check (board, board->active_color))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq: checkmate?
 *
 * Returns +true+ if the king of the color that has the turn is in checkmate, +false otherwise+.
 */
VALUE
board_king_in_checkmate (VALUE self)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  if (king_in_checkmate (board, board->active_color))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq: active_color
 *
 * Returns the active color: +false+ means white turn, +true+ means black turn.
 */
VALUE
board_active_color (VALUE self)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  if (board->active_color)
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq: halfmove_clock
 *
 * Returns the halfmove clock that is the number of halfmoves since the last pawn advance or capture. This is used to determine if a draw can be claimed under the fifty-move rule.
 */
VALUE
board_halfmove_clock (VALUE self)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  return INT2FIX (board->halfmove_clock);
}

/*
 * call-seq: fullmove_number
 *
 * Returns the fullmove number: the number of the full move. It starts at 1, and is incremented after black's move.
 */
VALUE
board_fullmove_number (VALUE self)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  return INT2FIX (board->fullmove_number);
}

/*
 * call-seq: to_fen
 *
 * Return the FEN string of the board.
 */
VALUE
board_to_fen (VALUE self)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  char *fen = to_fen (board);
  VALUE rb_fen = rb_str_new2 (fen);
  free (fen);
  return rb_fen;
}

/*
 * call-seq: to_s
 *
 * Board to string.
 */
VALUE
board_to_s (VALUE self)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  char *s = print_board (board);
  VALUE rb_s = rb_str_new2 (s);
  free (s);
  return rb_s;
}

// INIT

void
Init_chess ()
{
  init_chess_library ();
  VALUE chess = rb_define_module ("Chess");

  /*
   * This exception will be raised​when making an illegal move.
   */
  illegal_move_error = rb_define_class_under (chess, "IllegalMoveError", rb_eStandardError);

  /*
   * This class rappresents a collection of boards of a single chess game.
   */
  VALUE game = rb_define_class_under (chess, "CGame", rb_cObject);
  rb_define_alloc_func (game, game_alloc);
  rb_define_method (game, "move", game_move, 4);
  rb_define_method (game, "move2", game_move2, 3);
  rb_define_method (game, "move3", game_move3, 3);
  rb_define_method (game, "[]", game_boards, 1);
  rb_define_method (game, "board", game_last_board, 0);
  rb_define_method (game, "moves", game_moves, 0);
  rb_define_method (game, "full_moves", game_full_moves, 0);
  rb_define_method (game, "size", game_size, 0);
  rb_define_method (game, "result", game_result, 0);

  /*
   * This class rappresents a chess board.
   * The rappresentation of the board use _bitboards_ where each bit represents a game position or state, designed for optimization of speed and/or memory or disk use in mass calculations.
   * This ensures a fast library.
   */
  board_klass = rb_define_class_under (chess, "Board", rb_cObject);
  rb_define_method (board_klass, "check?", board_king_in_check, 0);
  rb_define_method (board_klass, "checkmate?", board_king_in_checkmate, 0);
  rb_define_method (board_klass, "active_color", board_active_color, 0);
  rb_define_method (board_klass, "halfmove_clock", board_halfmove_clock, 0);
  rb_define_method (board_klass, "fullmove_number", board_fullmove_number, 0);
  rb_define_method (board_klass, "to_fen", board_to_fen, 0);
  rb_define_method (board_klass, "to_s", board_to_s, 0);
}