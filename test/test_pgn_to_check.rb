# :nodoc:
require 'test_helper'

class ChessTest < Test::Unit::TestCase
  TestHelper.pgns('to_check').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_game_#{name}" do
      assert_nothing_raised(Chess::IllegalMoveError) do
        pgn = Chess::Pgn.new(file)
        game = Chess::Game.new
        pgn.moves.each do |m|
          mm = game.move(m)
        end
      end
    end
  end

end