/*
 * chess - a fast library to play chess in Ruby
 *
 * Copyright (c) 2011-2018, Enrico Pilotto <epilotto@gmx.com>
 * This code is under LICENSE LGPLv3
 */

#include "board.h"

// Initialize the board
void
init_board (Board *board)
{
  memcpy (board->placement, "RNBQKBNRPPPPPPPP\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0pppppppprnbqkbnr", 64);
  board->active_color = WHITE;
  board->castling = 0x1111;
  board->en_passant = -1;
  board->halfmove_clock = 0;
  board->fullmove_number = 1;
  // initialize bitboards
  board->pawns[WHITE]   = 0x000000000000ff00;
  board->pawns[BLACK]   = 0x00ff000000000000;
  board->rooks[WHITE]   = 0x0000000000000081;
  board->rooks[BLACK]   = 0x8100000000000000;
  board->knights[WHITE] = 0x0000000000000042;
  board->knights[BLACK] = 0x4200000000000000;
  board->bishops[WHITE] = 0x0000000000000024;
  board->bishops[BLACK] = 0x2400000000000000;
  board->queens[WHITE]  = 0x0000000000000008;
  board->queens[BLACK]  = 0x0800000000000000;
  board->king[WHITE]    = 0x0000000000000010;
  board->king[BLACK]    = 0x1000000000000000;
  set_occupied (board);
}

// Set helpers bitboards (white pieces, black pieces, occupied squares).
void
set_occupied (Board *board)
{
  board->pieces[WHITE] = board->pawns[WHITE]
    | board->rooks[WHITE]
    | board->knights[WHITE]
    | board->bishops[WHITE]
    | board->queens[WHITE]
    | board->king[WHITE];
  board->pieces[BLACK] = board->pawns[BLACK]
    | board->rooks[BLACK]
    | board->knights[BLACK]
    | board->bishops[BLACK]
    | board->queens[BLACK]
    | board->king[BLACK];
  board->occupied = board->pieces[WHITE] | board->pieces[BLACK];
}

// Board to string.
char*
print_board (Board *board)
{
  char *s = (char *) malloc (251);
  int si = 0;
  for (int i = 7; i >= 0; i--) // rank => top to bottom
    {
      sprintf (&s[si], "\e[37m%d\e[0m ", i + 1);
      si += 11;
      for (int j = 0; j < 8; j++) // file => left to right
        {
          char piece = board->placement[8 * i + j];
          sprintf (&s[si], "%c ", piece == 0 ? '.' : piece);
          si += 2;
        }
      sprintf (&s[si], "\n");
      si++;
    }
  sprintf (&s[si], "\e[37m  a b c d e f g h\e[0m");
  return s;
}

// Given a square returns its color (white is 0, black is 1).
int
get_color (Board *board, int square)
{
  int piece = board->placement[square];
  if (piece)
    if (piece < 97)
      return WHITE;
    else
      return BLACK;
  else
    return -1;
}

// Given a piece returns the piece's bitboard.
bboard*
get_piece_bitboard (Board *board, char piece)
{
  // TODO: we can avoid this swith using an array of boards, the index is the piece type
  switch (piece)
    {
    case 'P': return &board->pawns[WHITE];
    case 'p': return &board->pawns[BLACK];
    case 'R': return &board->rooks[WHITE];
    case 'r': return &board->rooks[BLACK];
    case 'N': return &board->knights[WHITE];
    case 'n': return &board->knights[BLACK];
    case 'B': return &board->bishops[WHITE];
    case 'b': return &board->bishops[BLACK];
    case 'Q': return &board->queens[WHITE];
    case 'q': return &board->queens[BLACK];
    case 'K': return &board->king[WHITE];
    case 'k': return &board->king[BLACK];
    default:  return 0;
    }
}

// Given a square (0..63) retuns the piece's bitboard in that square.
bboard*
get_bitboard (Board *board, int square)
{
  return get_piece_bitboard (board, board->placement[square]);
}

// Returns the bitboard with the xray that start from square. If only_attack use
// only pawns attack squares.
bboard
xray (Board *board, int from, bool only_attack)
{
  switch (board->placement[from])
    {
    case 'P':
      if (only_attack)
        return xray_attack_white_pawn (from);
      return xray_white_pawn (board->occupied, from);
    case 'p':
      if (only_attack)
        return xray_attack_black_pawn (from);
      return xray_black_pawn (board->occupied, from);
    case 'R': return xray_rook (board->occupied, from);
    case 'r': return xray_rook (board->occupied, from);
    case 'N': return xray_knight (from);
    case 'n': return xray_knight (from);
    case 'B': return xray_bishop (board->occupied, from);
    case 'b': return xray_bishop (board->occupied, from);
    case 'Q': return xray_queen (board->occupied, from);
    case 'q': return xray_queen (board->occupied, from);
    case 'K': return xray_king (from);
    case 'k': return xray_king (from);
    default:  return 0;
    }
}


// Returns the bitboard with all the xray that start from the color pieces.
bboard
all_xray (Board *board, int color, bool only_attack)
{
  bboard x = EMPTY_BOARD; // xray for all pieces
  for (int i = 0; i < 64; i++)
    {
      int piece_color = get_color (board, i);
      // Calculate xray for pieces of color [c]
      if (piece_color == color)
        x |= xray (board, i, only_attack);
    }
  return x;
}

// Returns the bitboard with all the xray that start from the color pieces with
// friend squares removed.
bboard
all_xray_without_friends (Board *board, int color, bool only_attack)
{
  return all_xray (board, color, only_attack) & ~board->pieces[color];
}

// Returns a new board with the piece in square position removed.
void
remove_piece (Board *board, int square, Board *new_board)
{
  memcpy (new_board, board, sizeof (Board));
  if (board->placement[square])
    {
      *(get_bitboard (new_board, square)) = EMPTY_BOARD;
      new_board->placement[square] = 0;
      set_occupied (new_board);
    }
}

// Returns the squares of the same pieces of color and its size that can capture
// the square. If piece_filter consider only that kind of pieces.
int
same_pieces_that_can_capture_a_square (Board *board, int color, int square, int *pieces, char piece_filter)
{
  int index = 0;
  char p;
  Board new_board;
  for (int i = 0; i < 64; i++)
    if (get_color (board, i) == color)
      {
        p = board->placement[i];
        if ((!piece_filter || p == piece_filter) && (xray (board, i, FALSE) & (1ULL << square)))
          if (try_move (board, i, square, 'Q', &new_board, 0, 0))
            {
              pieces[index] = i;
              index++;
            }
      }
  return index;
}

// Returns true if square can be captured from color pieces.
bool
capture (Board *board, int color, int square)
{
  return all_xray_without_friends (board, color, FALSE) & (1ULL << square) ? TRUE : FALSE;
}

// Returns true if there is a piece that can capture the square without put its
// king in check.
bool
pieces_can_safe_capture (Board *board, int color, int square)
{
  int pieces[16];
  return same_pieces_that_can_capture_a_square (board, color, square, pieces, 0) > 0;
}

// Returns true if the king color is in check.
bool
king_in_check (Board *board, int color)
{
  return all_xray_without_friends (board, !color, TRUE) & board->king[color] ? TRUE : FALSE;
}

// Returns true if the king color is in check. Assumes that the king is in
// check.
bool
king_in_checkmate (Board *board, int color)
{
  int king_square = square2 (board->king[color]);
  bboard king = xray_king (king_square) & ~board->pieces[color];
  Board board_without_king;
  remove_piece (board, king_square, &board_without_king); // King can't be auto-shielded xray
  bboard attackers = all_xray (&board_without_king, !color, TRUE);
  // If king can move return false
  if (king & ~attackers)
    return FALSE;
  int pieces[16];
  int n = same_pieces_that_can_capture_a_square (board, !color, king_square, pieces, 0);
  if (n == 0) return FALSE;
  // If king has more than 1 attacker return true
  if (n > 1) return TRUE;
  int attacker = pieces[0];
  // Test if there is a piece that can capture the attacker without put its king in check return false
  if (pieces_can_safe_capture (board, color, attacker))
    return FALSE;
  // Test if there is an en passant capture
  if (have_en_passant (board, attacker + 1, board->en_passant) ||
      have_en_passant (board, attacker - 1, board->en_passant))
    return FALSE;
  // Test if attack can be shielded
  bboard slide = EMPTY_BOARD;
  int i;
  for (i = 0; i < 8; i++)
    {
      slide = sliding_attacks (1ULL << attacker, FULL_BOARD, i);
      if (slide & 1ULL << king_square)
        break;
    }
  bboard attack = xray (board, attacker, TRUE) & slide;
  bboard defend = all_xray_without_friends (board, color, FALSE);
  bboard shield = attack & defend;
  if (shield)
    {
      int s[64];
      int n;
      squares (shield, s, &n);
      for (i = 0; i < n; i++)
        if (pieces_can_safe_capture (board, color, s[i]))
          return FALSE;
    }
  return TRUE;
}

// Returns true if the color pieces are in stale.
bool
stalemate (Board *board, int color)
{
  int s[64];
  int n;
  Board new_board;
  for (int i = 0; i < 64; i++)
    if (get_color (board, i) == color)
      {
        bboard b = xray (board, i, FALSE) & ~board->pieces[color];
        // if piece in i can move try move it
        if (b)
          {
            squares (b, s, &n);
            for (int j = 0; j < n; j++)
              if (try_move (board, i, s[j], 'Q', &new_board, 0, 0))
                return FALSE;
          }
      }
  return TRUE;
}

// Returns true if there are insufficient material to make a checkmate.
bool
insufficient_material (Board *board)
{
  if (board->queens[WHITE] || board->queens[BLACK]
      || board->rooks[WHITE] || board->rooks[BLACK]
      || board->pawns[WHITE] || board->pawns[BLACK])
    return FALSE;
  // King vs king
  if (!board->bishops[WHITE] && !board->bishops[BLACK]
      && !board->knights[WHITE] && !board->knights[BLACK])
    return TRUE;
  // King knight vs king
  if (!board->bishops[WHITE] && !board->bishops[BLACK])
    {
      if ((has_only_one_one(board->knights[WHITE]) && !board->knights[BLACK])
          || (has_only_one_one(board->knights[BLACK]) && !board->knights[WHITE]))
        return TRUE;
    }
  // King bishops vs king bishops
  if (!board->knights[WHITE] && !board->knights[BLACK])
    {
      if ((only_white_squares(board->bishops[WHITE]) && only_white_squares(board->bishops[BLACK]))
          || (only_black_squares(board->bishops[WHITE]) && only_black_squares(board->bishops[BLACK])))
        return TRUE;
    }
  // TODO more bishops on same color square
  return FALSE;
}

// Returns true if on the board there are only the two kings.
bool
only_kings (Board *board)
{
  return !board->pawns[WHITE]   && !board->pawns[BLACK] &&
         !board->rooks[WHITE]   && !board->rooks[BLACK] &&
         !board->knights[WHITE] && !board->knights[BLACK] &&
         !board->bishops[WHITE] && !board->bishops[BLACK] &&
         !board->queens[WHITE]  && !board->queens[BLACK];
}

// Returns true if fifty move rule is available.
bool
fifty_move_rule (Board *board)
{
  return board->halfmove_clock >= 50;
}

// Returns true if the move from-to can not promote the pawn.
bool
invalid_promotion (Board *board, int from, int to)
{
  if (toupper (board->placement[from]) != 'P')
    return TRUE;
  if ((board->active_color ? 0xffffffffffffff00 : 0x00ffffffffffffff) & (1ULL << to))
    return TRUE;
  return FALSE;
}

// Returns true if the move from-to is pseudo legal.
bool
pseudo_legal_move (Board *board, int from, int to)
{
  // Piece in from square is the right color
  if (get_color (board, from) != board->active_color)
    return FALSE;
  // The move is a castling
  if (castling_type (board, from, to))
    return TRUE;
  // The move is en passant
  if (have_en_passant (board, from, to))
    return TRUE;
  // The move is legal?
  bboard x = xray (board, from, FALSE) & ~board->pieces[board->active_color];
  return x & (1ULL << to) ? TRUE : FALSE;
}

// Returns true if the move piece-dis-to is legal. Returns also [from, to]
// coordinate (0..63). Pseudo legal move check included.
bool
get_coord (Board *board, char piece, const char *disambiguating, const char *to_coord, char promote_in, int *from, int *to)
{
  int count = 0;
  char file, rank;
  bboard x;
  *to = coord_to_square (to_coord);
  if (!piece)
    piece = 'P';
  for (int i = 0; i < 64; i++)
    {
      int c = get_color (board, i);
      if (c == board->active_color && piece == toupper (board->placement[i]))
        {
          x = xray (board, i, FALSE) & ~board->pieces[c];
          if (x & 1ULL << *to || have_en_passant (board, i, *to))
            {
              file = square_to_file (i);
              rank = square_to_rank (i);
              if (!disambiguating
                  || disambiguating[0] == file
                  || disambiguating[0] == rank
                  || (disambiguating[0] == file && disambiguating[1] == rank))
                {
                  Board new_board;
                  if (try_move (board, i, *to, promote_in, &new_board, 0, 0))
                    {
                      *from = i;
                      count++;
                    }
                }
            }
        }
    }
  if (count != 1)
    {
      *from = *to = 0;
      return FALSE;
    }
  // For compatibility: if pawn capture a file disambiguating is required
  if (piece == 'P' && !disambiguating && square_to_file (*from) != square_to_file (*to))
    {
      *from = *to = 0;
      return FALSE;
    }
  return TRUE;
}

// Returns true if the move from-to can be performed. Returns also the short
// algebraic chess notation of the move and the captured piece if occured.
// Assume that the move is pseudo legal. Do not consider castling, this is
// handled separately.
bool
try_move (Board *board, int from, int to, char promote_in, Board *new_board, char **move_done, char *capture)
{
  memcpy (new_board, board, sizeof (Board));
  *(get_bitboard (new_board, from)) ^= 1ULL << from;
  *(get_bitboard (new_board, from)) ^= 1ULL << to;
  if (capture)
    *capture = new_board->placement[to];
  int ep = have_en_passant (new_board, from, to);
  if (new_board->placement[to])
    *(get_bitboard (new_board, to)) ^= 1ULL << to;
  // En passant check
  else if (ep)
    {
      if (capture)
        *capture = new_board->placement[ep];
      *(get_bitboard (new_board, ep)) ^= 1ULL << ep;
      new_board->placement[ep] = 0;
    }
  // Change placement
  new_board->placement[to] = new_board->placement[from];
  new_board->placement[from] = 0;
  // Promotion check
  if (require_a_promotion (new_board))
    promote (new_board, to, promote_in);
  else
    promote_in = 0;
  // Set occupied pieces in new board
  set_occupied (new_board);
  // If king is in check can't move!
  if (king_in_check (new_board, get_color (board, from)))
    return FALSE;
  // Get short algebraic chess notation of the move
  if (move_done && capture)
    *move_done = get_notation (board, from, to, *capture, ep, promote_in, 0, 0);
  return TRUE;
}

// Returns the short algebraic chess notation of the move from-to. Do not
// perform the check to add mate symbols like '+' or '#'. This check is done on
// `apply_move` method.
char*
get_notation (Board *board, int from, int to, int capture, int ep, char promotion, int check, int checkmate)
{
  // Get short algebraic chess notation
  int i = 0;
  char *notation = (char *) malloc (9);
  char piece = toupper (board->placement[from]);
  if (piece != 'P')
    {
      notation[i] = piece;
      i++;
    }
  else if (capture || ep)
    {
      notation[i] = square_to_file (from);
      i++;
    }
  // Disambiguating
  if (piece != 'P')
    {
      int pieces[9];
      int n = same_pieces_that_can_capture_a_square (board, board->active_color, to, pieces, board->placement[from]);
      if (n > 1)
        {
          char from_file = square_to_file (from);
          char from_rank = square_to_rank (from);
          char disc_file, disc_rank;
          int disc_file_added = 0;
          int disc_rank_added = 0;
          int j = 0;
          for (j = 0; j < n; j++)
            {
              disc_file = square_to_file (pieces[j]);
              disc_rank = square_to_rank (pieces[j]);
              if (from_file != disc_file || from_rank != disc_rank)
                {
                  if (from_file != disc_file && !disc_file_added)
                    {
                      notation[i] = from_file;
                      i++;
                      disc_file_added = 1;
                    }
                  if (from_file == disc_file && from_rank != disc_rank && !disc_rank_added)
                    {
                      notation[i] = from_rank;
                      i++;
                      disc_rank_added = 1;
                    }
                }
            }
          // Swap file and rank disambiguating char if number is before the letter.
          if (disc_file_added && disc_rank_added && notation[i-2] < notation[i-1])
            {
              int tmp = notation[i-2];
              notation[i-2] = notation[i-1];
              notation[i-1] = tmp;
            }
        }
    }
  if (capture || ep)
    {
      notation[i] = 'x';
      i++;
    }
  notation[i] = square_to_file (to);
  notation[i+1] = square_to_rank (to);
  i += 2;
  if (ep)
    {
      notation[i] = 'e';
      notation[i+1] = 'p';
      i += 2;
    }
  if (promotion)
    {
      notation[i] = '=';
      notation[i+1] = promotion;
      i += 2;
    }
  if (checkmate)
    {
      notation[i] = '#';
      i++;
    }
  else if (check)
    {
      notation[i] = '+';
      i++;
    }
  notation[i] = '\0';
  return notation;
}

// Returns the fen string that rappresent the board.
char*
to_fen (Board *board)
{
  // 1. Placement
  char placement[65];
  int cur = 0;
  char p, pp = '-';
  for (int i = 7; i >= 0; i--)
    {
      for (int j = 0; j < 8; j++)
        {
          p = board->placement[j+i*8];
          if (p == '\0' && p == pp)
            {
              placement[cur-1] += 1;
            }
          else if (p == '\0')
            {
              placement[cur] = '1';
              cur++;
            }
          else
            {
              placement[cur] = p;
              cur++;
            }
          pp = p;
        }
      if (p == '\0') pp = '-';
      if (i > 0)
        {
          placement[cur] = '/';
          cur++;
        }
    }
  placement[cur] = '\0';

  // 2. Active color
  char active_color;
  if (board->active_color)
    active_color = 'b';
  else
    active_color = 'w';

  // 3. Castling availability
  char *castling = castling_to_s (board->castling);

  // 4. En passant target square
  char *ep;
  if (board->en_passant >= 0)
    ep = square_to_coord (board->en_passant);
  else
    {
      ep = (char *) malloc (2);
      strcpy (ep, "-");
    }

  // 5. Halfmove clock
  // > board->halfmove_clock

  // 6. Fullmove number
  // > board->fullmove_number

  // join all in fen string
  char *fen = (char *) malloc (80);
  sprintf (fen, "%s %c %s %s %d %d", placement, active_color, castling, ep, board->halfmove_clock, board->fullmove_number);

  free (castling);
  free (ep);

  return fen;
}
