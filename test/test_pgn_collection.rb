require 'test_helper'

class ChessTest < Minitest::Test
  TestHelper.pgns('valid').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_pgn_#{name}" do
      pgn = Chess::Pgn.new(file)
      game = Chess::Game.new
      pgn.moves.each do |m|
        game.move(m)
        assert(game.board.check?) if m.match?(/\+$/)
        assert(game.board.checkmate?) if m.match?(/\#$/)
      end
    end
    define_method "test_pgn_result_#{name}" do
      pgn = Chess::Pgn.new(file)
      game = Chess::Game.load_pgn(file)
      assert_equal(pgn.result, game.result)
    end
  end
end
