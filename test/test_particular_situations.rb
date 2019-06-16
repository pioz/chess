require 'test_helper'

class ChessTest < Minitest::Test
  def test_cant_castling
    game = Chess::Game.load_fen('2r1k2r/pR2p2p/2pn2Nb/P2p1p2/8/1P2P3/2PP4/2BQK3 b k - 1 3')
    assert_raises(Chess::IllegalMoveError) do
      game << 'O-O'
    end
    game = Chess::Game.load_fen('2r1k2r/pR2p2p/2pn3b/P2p1p2/8/1P2P3/2PP4/2BQK3 b k - 1 3')
    game << 'O-O'
  end
end
