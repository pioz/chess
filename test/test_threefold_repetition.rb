require 'test_helper'

class ChessTest < Minitest::Test

  TestHelper.pgns('threefold_repetition').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_threefold_repetition_#{name}" do
      pgn = Chess::Pgn.new(file)
      game = Chess::Game.new(pgn.moves)
      assert(game.threefold_repetition?)
    end
  end

end
