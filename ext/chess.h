/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2012, Enrico Pilotto <enrico@megiston.it>
 * This code is under LICENSE LGPLv3
 */

#include "ruby.h"
#include "game.h"

// Game

VALUE game_alloc (VALUE class);
VALUE game_set_fen (VALUE self, VALUE fen);
VALUE game_move (VALUE self, VALUE rb_piece, VALUE rb_disambiguating, VALUE rb_to_coord, VALUE rb_promote_in);
VALUE game_move2 (VALUE self, VALUE rb_from, VALUE rb_to, VALUE rb_promote_in);
VALUE game_move3 (VALUE self, VALUE rb_from, VALUE rb_to, VALUE rb_promote_in);
VALUE game_resign (VALUE self, VALUE color);
VALUE game_draw (VALUE self);
VALUE game_boards (VALUE self, VALUE index);
VALUE game_current_board (VALUE self);
VALUE game_moves (VALUE self);
VALUE game_full_moves (VALUE self);
VALUE game_threefold_repetition (VALUE self);
VALUE game_result (VALUE self);
VALUE game_size (VALUE self);
VALUE game_each (VALUE self);
VALUE game_rollback (VALUE self);
VALUE game_to_s (VALUE self);

// Board

VALUE board_placement (VALUE self);
VALUE board_get_piece (VALUE self, VALUE square);
VALUE board_king_in_check (VALUE self);
VALUE board_king_in_checkmate (VALUE self);
VALUE board_stalemate (VALUE self);
VALUE board_insufficient_material (VALUE self);
VALUE board_fifty_move_rule (VALUE self);
VALUE board_active_color (VALUE self);
VALUE board_halfmove_clock (VALUE self);
VALUE board_fullmove_number (VALUE self);
VALUE board_to_fen (VALUE self);
VALUE board_to_s (VALUE self);

// INIT

void Init_chess ();
