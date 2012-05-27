# :nodoc:
require 'test_helper'

class ChessTest < Test::Unit::TestCase
  TestHelper.pgns('valid').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_game_#{name}" do
      assert_nothing_raised(Chess::IllegalMoveError) do
        pgn = Chess::Pgn.new(file)
        game = Chess::Game.new
        pgn.moves.each do |m|
          mm = game.move(m)
          mm.gsub!(/\+$/, '') if m !~ /\+$/
          mm.gsub!('ep', '') if m !~ /ep/
          assert_equal(m, mm)
          assert(game.board.check?) if m =~ /\+$/
          assert(game.board.checkmate?) if m =~ /\#$/
        end
      end
    end
  end

end
