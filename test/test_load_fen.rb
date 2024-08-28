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
end
