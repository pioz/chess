require 'test_helper'

class ChessTest < Minitest::Test
  def test_bad_notation_error
    game = Chess::Game.new
    assert_raises(Chess::BadNotationError) do
      game << 'gg'
    end
  end

  def test_invalid_pgn_format_error
    assert_raises(Chess::InvalidPgnFormatError) do
      Chess::Pgn.new('test/pgn_collection/invalid/0001.pgn')
    end
  end

  def test_invalid_fen_format_error
    assert_raises(Chess::InvalidFenFormatError) do
      Chess::Game.load_fen('invalid')
    end
  end
end
