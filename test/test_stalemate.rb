require 'test_helper'

class ChessTest < Minitest::Test
  TestHelper.pgns('stalemate').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_stalemate_#{name}" do
      pgn = Chess::Pgn.new(file)
      game = Chess::Game.new(pgn.moves)
      assert(game.board.stalemate?)
      assert_equal('1/2-1/2', game.result)
    end
  end
end
