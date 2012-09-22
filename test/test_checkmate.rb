require 'test_helper'

class ChessTest < Test::Unit::TestCase

  TestHelper.pgns('checkmate').each do |file|
    name = File.basename(file, '.pgn')
    win = file.include?('white') ? 'white' : 'black'
    define_method "test_#{win}_checkmate_#{name}" do
      assert_nothing_raised(Chess::IllegalMoveError) do
        pgn = Chess::Pgn.new(file)
        game = Chess::Game.new(pgn.moves)
        assert(game.board.checkmate?)
        if file.include?('white_wins')
          assert_equal(game.active_player, :black)
        elsif file.include?('black_wins')
          assert_equal(game.active_player, :white)
        end
      end
    end
  end

end
