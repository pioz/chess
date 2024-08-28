require 'test_helper'

class ChessTest < Minitest::Test
  TestHelper.pgns('checkmate').each do |file|
    name = File.basename(file, '.pgn')
    win = file.include?('white') ? 'white' : 'black'

    define_method :"test_#{win}_checkmate_#{name}" do
      pgn = Chess::Pgn.new(file)
      game = Chess::Game.new(pgn.moves)

      assert(game.board.checkmate?)
      if file.include?('white_won')
        assert_equal('1-0', game.result)
        assert_equal(:black, game.active_player)
      elsif file.include?('black_won')
        assert_equal('0-1', game.result)
        assert_equal(:white, game.active_player)
      end
    end
  end
end
