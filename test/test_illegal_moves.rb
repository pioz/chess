require 'test_helper'

class ChessTest < Minitest::Test
  def test_illegal_moves
    game = Chess::Game.new

    %w[Qf6 Rd4 Nc3=Q].each do |move|
      assert_raises(Chess::IllegalMoveError) do
        game << move
      end
    end
  end
end
