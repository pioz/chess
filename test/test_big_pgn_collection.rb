require 'test_helper'

class ChessTest < Test::Unit::TestCase
  TestHelper.pgns('.', TestHelper::BIG_PGN_COLLECTION).each do |file|
    define_method "test_game_#{file}" do
      assert_nothing_raised(Chess::IllegalMoveError) do
        pgn = Chess::Pgn.new(file)
        game = Chess::Game.new(pgn.moves)
        if pgn.moves.last =~ /\#$/
          assert_equal(pgn.moves.last, game.moves.last)
        end
      end
    end
  end

end
