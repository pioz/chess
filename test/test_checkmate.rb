require 'test_helper'

class ChessTest < Minitest::Test

  TestHelper.pgns('checkmate').each do |file|
    name = File.basename(file, '.pgn')
    win = file.include?('white') ? 'white' : 'black'
    define_method "test_#{win}_checkmate_#{name}" do
      pgn = Chess::Pgn.new(file)
      game = Chess::Game.new(pgn.moves)
      assert(game.board.checkmate?)
      if file.include?('white_won')
        assert_equal(game.result, '1-0')
        assert_equal(game.active_player, :black)
      elsif file.include?('black_won')
        assert_equal(game.result, '0-1')
        assert_equal(game.active_player, :white)
      end
    end
  end

end
