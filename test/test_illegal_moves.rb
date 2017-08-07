require 'test_helper'

class ChessTest < Minitest::Test

  def test_illegal_moves
    game = Chess::Game.new
    %w(Qf6 Rd4).each do |move|
      e = assert_raises(Chess::IllegalMoveError) do
        game << move
      end
      assert_equal "Illegal move '#{move}'", e.message
    end
  end

end
