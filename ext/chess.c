/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2012, Enrico Pilotto <epilotto@gmx.com>
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
  Game *g = init_game ();
  return Data_Wrap_Struct (class, 0, free_game, g);
  // VALUE tdata = Data_Wrap_Struct (class, 0, free_game, g);
  // rb_obj_call_init (tdata, 0, 0);
  // return tdata;
}

/*
 * call-seq: set_fen!(fen)
 *
 * Set the game position with a FEN string.
 *
 * Parameters are:
 * +fen+:: the FEN (Forsyth–Edwards Notation) string notation used to set the
 *         game position.
 */
VALUE
game_set_fen (VALUE self, VALUE fen)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  set_fen (g, StringValuePtr (fen));
  return self;
}

/*
 * call-seq: move(piece, disambiguating, to_coord, promote_in)
 *
 * Make a move. This add a new Board in the Game.
 *
 * Parameters are:
 * +piece+:: the character of the moving piece <em>('P', 'R', 'N', 'B', 'Q', 'K')</em>.
 * +disambiguating+:: when two (or more) identical pieces can move to the same
 *                    square, the moving piece is uniquely identified by
 *                    specifying the piece's letter, followed by (in descending
 *                    order of preference):
 *                    * the file of departure (if they differ); or
 *                    * the rank of departure (if the files are the same but
 *                      the ranks differ); or
 *                    * both the rank and file (if neither alone is
 *                      sufficient to identify the piece—which occurs only in
 *                      rare cases where one or more pawns have promoted,
 *                      resulting in a player having three or more identical
 *                      pieces able to reach the same square).
 *                    Keep blank if no needed.
 * +to_coord+:: the square where the moving piece will <em>('a1', 'a2', ... , 'h7', 'h8')</em>.
 * +promote_in+:: the character of promotion piece <em>('R', 'N', 'B', 'Q')</em>.
 *                If no promotion occured, this param will be ignored. If no
 *                value is passed, 'Q' is the default.
 *
 * This method returns a string that represents the short algebraic chess
 * notation of the move or raise an IllegalMoveError if the move is illegal.
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
 * +from+:: the 2 character string representing the starting square of the
 *          moving piece <em>('a1', 'a2', ... , 'h7', 'h8')</em>.
 * +to+:: the 2 character string representing the ending square of the moving
 *        piece <em>('a1', 'a2', ... , 'h7', 'h8')</em>.
 * +promote_in+:: the character of promotion piece <em>('R', 'N', 'B', 'Q')</em>.
 *                If no promotion occured, this param will be ignored. If no
 *                value is passed, 'Q' is the default.
 *
 * This method returns a string that represents the short algebraic chess
 * notation of the move or raise an IllegalMoveError if the move is illegal.
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
 * Each square on the chessboard is represented by an integer according to the
 * following scheme:
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
 *
 * Parameters are:
 * +from+:: the integer representing the starting square of the moving piece.
 * +to+:: the integer representing the ending square of the moving piece.
 * +promote_in+:: the character of promotion piece <em>('R', 'N', 'B', 'Q')</em>.
 *                If no promotion occured, this param will be ignored. If no
 *                value is passed, 'Q' is the default.
 *
 * This method returns a string that represents the short algebraic chess
 * notation of the move or raise an IllegalMoveError if the move is illegal.
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
 * call-seq: resign(color)
 *
 * The game result is set to '1-0' if +color+ is "black", otherwise is set to
 * '0-1' if color is "white".
 *
 * Parameters are:
 * +color+:: the color of the player who resigns the game; it can be +:white+ or
 *           +:black+.
 */
VALUE
game_resign (VALUE self, VALUE color)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  const char *c;
  if (TYPE (color) == T_SYMBOL)
    c = rb_id2name (SYM2ID (color));
  else
    c = StringValuePtr (color);
  if (strcmp (c, "black") == 0)
    g->result = WHITE_WON;
  else if (strcmp (c, "white") == 0)
    g->result = BLACK_WON;
  return Qnil;
}

/*
 * call-seq: draw
 *
 * The game result is set to draw.
 */
VALUE
game_draw (VALUE self)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  g->result = DRAW;
  return Qnil;
}

/*
 * call-seq: [n]
 *
 * Returns the +n+-th Board of the Game or +nil+ if the +n+-th Board does not
 * exist.
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
 * Returns the current Board of the Game (the current chess position of the
 * game).
 */
VALUE
game_current_board (VALUE self)
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
  VALUE moves = rb_ary_new ();
  for (int i = 0; i < g->current; i++)
    rb_ary_push (moves, rb_str_new2 (g->moves[i]));
  return moves;
}

/*
 * call-seq: coord_moves
 *
 * Returns the array with all moves done in coordinate chess notation <em>(es: b1c3)</em>.
 */
VALUE
game_coord_moves (VALUE self)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  VALUE moves = rb_ary_new ();
  for (int i = 0; i < g->current; i++)
    rb_ary_push (moves, rb_str_new2 (g->coord_moves[i]));
  return moves;
}

/*
 * :nodoc:
 */
VALUE
game_full_moves (VALUE self)
{
  printf ("DEPRECATION WARNING: `full_moves` is deprecated and will be removed, please use `coord_moves` to get the array with all moves done in coordinate chess notation.\n");
  return game_coord_moves (self);
}

/*
 * call-seq: threefold_repetition?
 *
 * Returns +true+ if a player can claim draw by the threefold repetition rule,
 * +false+ otherwise.
 */
VALUE
game_threefold_repetition (VALUE self)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  if (threefold_repetition (g))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq: result
 *
 * Returns the result of the game:
 * *:: game in progress;
 * 1-0:: white won;
 * 0-1:: black won;
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
 * call-seq: each { |board, move, coord_move, index| block }
 *
 * Calls +block+ once for each +board+ in self, passing that
 * +board+, +move+, +coord_move+ and +index+ as parameters. Return self.
 * If no block is given, the array of game moves is returned instead.
 */
VALUE
game_each (VALUE self)
{
  if (!rb_block_given_p ())
    return game_moves(self);
  Game *g;
  Data_Get_Struct (self, Game, g);
  for (int i = 0; i < g->current; i++)
    rb_yield_values (4,
                     Data_Wrap_Struct (board_klass, 0, 0, get_board (g, i)),
                     rb_str_new2 (g->moves[i]),
                     rb_str_new2 (g->coord_moves[i]),
                     INT2FIX (i));
  return self;
}

/*
 * call-seq: rollback!
 *
 * Rollback last move.
 */
VALUE
game_rollback (VALUE self)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  rollback (g);
  return self;
}

/*
 * call-seq: to_s
 *
 * Current board to string.
 */
VALUE
game_to_s (VALUE self)
{
  Game *g;
  Data_Get_Struct (self, Game, g);
  Board *b = get_board (g, g->current-1);
  char *s = print_board (b);
  VALUE rb_s = rb_str_new2 (s);
  free (s);
  return rb_s;
}

// Board

/*
 * call-seq: placement { |piece, index| block }
 *
 * Calls +block+ once for each square in the board, passing the
 * +piece+ in the square and the +index+ as parameters. Return self.
 * If no block is given, the array of pieces is returned instead.
 */
VALUE
board_placement (VALUE self)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  if (!rb_block_given_p ())
    {
      VALUE placement = rb_ary_new ();
      for (int i = 0; i < 64; i++)
        {
          char piece = board->placement[i];
          rb_ary_push (placement, rb_str_new (&piece, 1));
        }
      return placement;
    }
  else
    {
      for (int i = 0; i < 64; i++)
        {
          char piece = board->placement[i];
          rb_yield_values (2, rb_str_new (&piece, 1), INT2FIX (i));
        }
      return self;
    }
}

/*
 * call-seq: [square]
 *
 * Returns the piece on the +square+ of the chessboard. If there is no piece or
 * the square is not valid return +nil+.
 *
 * Each square on the chessboard is represented by an integer according to the
 * following scheme:
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
 *
 * Parameters are:
 * +square+:: the square of the board. Can be an integer between 0 and 63 or a
 *            string like 'a2', 'c5'...
 */
VALUE
board_get_piece (VALUE self, VALUE square)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  int i;
  if (TYPE (square) == T_STRING)
    i = coord_to_square (StringValuePtr (square));
  else
    i = FIX2INT (square);
  char piece = board->placement[i];
  if (i < 0 || i > 63 || !piece)
    return Qnil;
  else
    return rb_str_new (&piece, 1);
}

/*
 * call-seq: check?
 *
 * Returns +true+ if the king of the color that has the turn is in check,
 * +false+ otherwise.
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
 * Returns +true+ if the king of the color that has the turn is in checkmate,
 * +false+ otherwise.
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
 * call-seq: stalemate?
 *
 * Returns +true+ if the pieces of the color that has the turn are in stalemate,
 * +false+ otherwise.
 */
VALUE
board_stalemate (VALUE self)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  if (stalemate (board, board->active_color))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq: insufficient_material?
 *
 * Returns +true+ if the board has insufficient material to checkmate, +false+
 * otherwise.
 */
VALUE
board_insufficient_material (VALUE self)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  if (insufficient_material (board))
    return Qtrue;
  else
    return Qfalse;
}

/*
 * call-seq: fifty_move_rule?
 *
 * Returns +true+ if a player can claim draw by the fifty-move rule, +false+
 * otherwise.
 */
VALUE
board_fifty_move_rule (VALUE self)
{
  Board *board;
  Data_Get_Struct (self, Board, board);
  if (fifty_move_rule (board))
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
 * Returns the halfmove clock that is the number of halfmoves since the last
 * pawn advance or capture. This is used to determine if a draw can be claimed
 * under the fifty-move rule.
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
 * Returns the fullmove number: the number of the full move. It starts at 1, and
 * is incremented after black's move.
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
   * This class rappresents a collection of boards of a single chess game.
   */
  VALUE game = rb_define_class_under (chess, "CGame", rb_cObject);
  rb_define_alloc_func (game, game_alloc);
  rb_define_method (game, "set_fen!", game_set_fen, 1);
  rb_define_method (game, "move", game_move, 4);
  rb_define_method (game, "move2", game_move2, 3);
  rb_define_method (game, "move3", game_move3, 3);
  rb_define_method (game, "resign", game_resign, 1);
  rb_define_method (game, "draw", game_draw, 0);
  rb_define_method (game, "[]", game_boards, 1);
  rb_define_method (game, "current", game_current_board, 0);
  rb_define_method (game, "moves", game_moves, 0);
  rb_define_method (game, "coord_moves", game_coord_moves, 0);
  rb_define_method (game, "full_moves", game_full_moves, 0);
  rb_define_method (game, "threefold_repetition?", game_threefold_repetition, 0);
  rb_define_method (game, "result", game_result, 0);
  rb_define_method (game, "size", game_size, 0);
  rb_define_method (game, "each", game_each, 0);
  rb_define_method (game, "rollback!", game_rollback, 0);
  rb_define_method (game, "to_s", game_to_s, 0);
  rb_define_alias (game, "board", "current");

  /*
   * This class rappresents a chess board.
   * The rappresentation of the board use _bitboards_ where each bit represents
   * a game position or state, designed for optimization of speed and/or memory
   * or disk use in mass calculations.
   * This ensures a fast library.
   */
  board_klass = rb_define_class_under (chess, "Board", rb_cObject);
  rb_define_method (board_klass, "placement", board_placement, 0);
  rb_define_method (board_klass, "[]", board_get_piece, 1);
  rb_define_method (board_klass, "check?", board_king_in_check, 0);
  rb_define_method (board_klass, "checkmate?", board_king_in_checkmate, 0);
  rb_define_method (board_klass, "stalemate?", board_stalemate, 0);
  rb_define_method (board_klass, "insufficient_material?", board_insufficient_material, 0);
  rb_define_method (board_klass, "fifty_rule_move?", board_fifty_move_rule, 0);
  rb_define_method (board_klass, "active_color", board_active_color, 0);
  rb_define_method (board_klass, "halfmove_clock", board_halfmove_clock, 0);
  rb_define_method (board_klass, "fullmove_number", board_fullmove_number, 0);
  rb_define_method (board_klass, "to_fen", board_to_fen, 0);
  rb_define_method (board_klass, "to_s", board_to_s, 0);

  /*
   * This exception will be raised when making an illegal move.
   */
  illegal_move_error = rb_define_class_under (chess, "IllegalMoveError", rb_eStandardError);
}
