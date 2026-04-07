require 'test_helper'

class ChessTest < Minitest::Test
  def test_fen_in_progress
    g = Chess::Game.load_fen('rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2')

    assert_equal 'P', g.board['e4']
    assert_equal 'p', g.board['c5']
    assert_equal 'K', g.board[4]
    assert_equal '*', g.result
    assert_equal :in_progress, g.status
  end

  def test_fen_white_won
    g = Chess::Game.load_fen('rnbqkbnr/1ppp1Qpp/8/p3p3/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 1 3')

    assert_equal 'Q', g.board['f7']
    assert_equal 'p', g.board['e5']
    assert_equal 'P', g.board[28]
    assert_equal '1-0', g.result
    assert_equal :white_won, g.status
  end

  def test_fen_black_won
    g = Chess::Game.load_fen('rnb1kbnr/pppp1ppp/4p3/8/5PPq/8/PPPPP2P/RNBQKBNR w KQkq - 1 3')

    assert_equal 'q', g.board['h4']
    assert_equal 'P', g.board['f4']
    assert_equal 'p', g.board[44]
    assert_equal '0-1', g.result
    assert_equal :black_won, g.status
  end

  def test_fen_stalemate
    g = Chess::Game.load_fen('8/6b1/8/8/8/n7/PP6/K7 w KQkq - 1 3')

    assert_equal 'b', g.board['g7']
    assert_equal 'n', g.board['a3']
    assert_equal 'K', g.board[0]
    assert_equal '1/2-1/2', g.result
    assert_equal :stalemate, g.status
  end

  def test_fen_castling_from
    g = Chess::Game.load_fen('2b1kbnr/rpq1pp1p/2n3p1/8/3Q4/2P5/PP3PPP/RN2KBNR w KQk - 0 9')
    g.move('Kd1')

    assert_equal '2b1kbnr/rpq1pp1p/2n3p1/8/3Q4/2P5/PP3PPP/RN1K1BNR b k - 1 9', g.board.to_fen
  end

  def test_fen_long_placement_no_overflow
    # Issue #35: placement strings >= 65 chars caused a stack buffer overflow
    g = Chess::Game.load_fen('r2q1rk1/1pp2pp1/p1np1n1p/2b1p3/2B1P1b1/P1NP1N2/1PP1QPPP/R1B1R1K1 w - - 2 10')
    g.move('h2h3')

    assert_equal 'r2q1rk1/1pp2pp1/p1np1n1p/2b1p3/2B1P1b1/P1NP1N1P/1PP1QPP1/R1B1R1K1 b - - 0 10', g.board.to_fen
  end

  def test_fen_long_placement_threefold_repetition
    # Issue #35: threefold_repetition? calls to_fen internally, exercising
    # the placement buffer in game.c with a long placement string.
    # Note: 3 full cycles (12 half-moves) needed because the FEN-loaded board
    # stores en_passant differently from move-generated boards.
    g = Chess::Game.load_fen('r2q1rk1/1pp2pp1/p1np1n1p/2b1p3/2B1P1b1/P1NP1N1P/1PP1QPP1/R1B1R1K1 b - - 0 10')
    3.times do
      g.move('Qd7')
      g.move('Qe3')
      g.move('Qd8')
      g.move('Qe2')
    end

    assert g.threefold_repetition?
  end

  def test_fen_max_length_fills_malloc_buffer
    # Exercises the malloc(104) boundary in to_fen: 71 placement + 1 space +
    # 1 active + 1 space + 4 castling + 1 space + 2 ep + 1 space +
    # 10 halfmove + 1 space + 10 fullmove = 103 chars + NUL = 104 bytes.
    fen = 'rnbqkbnr/pppppppp/nnnnnnnn/bbbbbbbb/BBBBBBBB/NNNNNNNN/PPPPPPPP/RNBQKBNR b KQkq e3 2147483647 2147483647'
    g = Chess::Game.load_fen(fen)
    result = g.board.to_fen

    assert_equal fen, result
    assert_equal 103, result.length
  end

  def test_fen_castling_to
    g = Chess::Game.load_fen('2b1kbnr/rpq1pp1p/2n3p1/8/3Q4/2P5/PP3PPP/RN2KBNR w KQk - 0 9')
    g.move('Qh8')

    assert_equal '2b1kbnQ/rpq1pp1p/2n3p1/8/8/2P5/PP3PPP/RN2KBNR b KQ - 0 9', g.board.to_fen
  end
end
