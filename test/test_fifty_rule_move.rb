require 'test_helper'

class ChessTest < Test::Unit::TestCase

  TestHelper.pgns('fifty_move_rule').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_fifty_rule_move_#{name}" do
      assert_nothing_raised(Chess::IllegalMoveError) do
        pgn = Chess::Pgn.new(file)
        game = Chess::Game.new(pgn.moves)
        assert(game.board.fifty_rule_move?)
      end
    end
  end

end
