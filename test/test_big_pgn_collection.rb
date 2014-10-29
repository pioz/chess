require 'test_helper'

class ChessTest < Minitest::Test
  TestHelper.pgns('.', TestHelper::BIG_PGN_COLLECTION).each do |file|
    define_method "test_big_pgn_#{file}" do
      pgn = Chess::Pgn.new(file)
      game = Chess::Game.new(pgn.moves)
      assert(game.checkmate?) if pgn.moves.last =~ /\#$/
    end
  end

end
