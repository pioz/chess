require 'test_helper'

class ChessTest < Test::Unit::TestCase

  TestHelper.pgns('threefold_repetition').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_threefold_repetition_#{name}" do
      assert_nothing_raised(Chess::IllegalMoveError) do
        pgn = Chess::Pgn.new(file)
        game = Chess::Game.new(pgn.moves)
        assert(game.threefold_repetition?)
      end
    end
  end

end
