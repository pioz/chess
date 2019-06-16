require 'test_helper'

class ChessTest < Minitest::Test
  TestHelper.pgns('invalid').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_invalid_pgn_#{name}" do
      assert_raises(Chess::InvalidPgnFormatError) do
        Chess::Pgn.new(file)
      end
    end
  end

  TestHelper.pgns('illegal').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_illegal_pgn_#{name}" do
      assert_raises(Chess::IllegalMoveError) do
        Chess::Pgn.new(file, check_moves: true)
      end
      Chess::Pgn.new(file)
    end
  end
end
