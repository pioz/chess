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

  def test_fen_castling_to
    g = Chess::Game.load_fen('2b1kbnr/rpq1pp1p/2n3p1/8/3Q4/2P5/PP3PPP/RN2KBNR w KQk - 0 9')
    g.move('Qh8')

    assert_equal '2b1kbnQ/rpq1pp1p/2n3p1/8/8/2P5/PP3PPP/RN2KBNR b KQ - 0 9', g.board.to_fen
  end

  # Regression: Board#to_fen used a 65-byte stack buffer (char placement[65])
  # that could only hold 64 placement chars + NUL. Any position whose
  # placement field reached 65 chars would overflow by one byte, clobber
  # the stack canary, and abort the process with SIGABRT. This is the
  # FEN from Lichess puzzle 9Rmqa (https://lichess.org/training/9Rmqa),
  # whose pre-move placement is 64 chars (fits the old buffer) but whose
  # post-h2h3 placement is 65 chars (overflows it).
  def test_fen_65_char_placement_after_move
    g = Chess::Game.load_fen('r2q1rk1/1pp2pp1/p1np1n1p/2b1p3/2B1P1b1/P1NP1N2/1PP1QPPP/R1B1R1K1 w - - 2 10')
    g.move('h2h3')
    fen = g.board.to_fen
    placement = fen.split(' ').first

    assert_equal 65, placement.length
    assert_equal 'r2q1rk1/1pp2pp1/p1np1n1p/2b1p3/2B1P1b1/P1NP1N1P/1PP1QPP1/R1B1R1K1 b - - 0 10', fen
  end

  # Same bug, different trigger: from the same 9Rmqa starting FEN, g2g3
  # also pushes the placement from 64 to 65 chars by splitting rank 3's
  # two-empty run around the pawn landing square.
  def test_fen_65_char_placement_alternative_trigger
    g = Chess::Game.load_fen('r2q1rk1/1pp2pp1/p1np1n1p/2b1p3/2B1P1b1/P1NP1N2/1PP1QPPP/R1B1R1K1 w - - 2 10')
    g.move('g2g3')
    fen = g.board.to_fen
    placement = fen.split(' ').first

    assert_equal 65, placement.length
    assert_equal 'r2q1rk1/1pp2pp1/p1np1n1p/2b1p3/2B1P1b1/P1NP1NP1/1PP1QP1P/R1B1R1K1 b - - 0 10', fen
  end

  # Safety margin: the theoretical max legal placement length is 71 chars
  # (every square occupied with pieces separated so no empty-square runs
  # collapse). load_fen does not validate piece counts, so we can build
  # a synthetic max-length position by alternating pawns and empties on
  # every rank. The new buffer must hold this without overflow.
  def test_fen_max_length_placement
    max_placement = 'P1P1P1P1/1P1P1P1P/P1P1P1P1/1P1P1P1P/p1p1p1p1/1p1p1p1p/p1p1p1p1/1p1p1p1p'
    assert_equal 71, max_placement.length

    g = Chess::Game.load_fen("#{max_placement} w - - 0 1")
    fen = g.board.to_fen

    assert_equal max_placement, fen.split(' ').first
  end
end
