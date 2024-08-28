require 'test_helper'

class UCICastrlingTest < Minitest::Test
  def test_uci_white_short_castling
    game = Chess::Game.new
    game.moves = %w[e4 c5 c3 d5 exd5 Qxd5 d4 Nf6 Nf3 e6 Be2 Be7 e1h1 Nc6]
    assert_equal '*', game.result
  end

  def test_uci_white_long_castling
    game = Chess::Game.new
    game.moves = %w[e4 c5 Nf3 Nc6 d4 cxd4 Nxd4 Nf6 Nc3 d6 Bg5 Qb6 Nb3 e6 Qd2 Be7 e1a1]
    assert_equal '*', game.result
  end

  def test_uci_black_short_castling
    game = Chess::Game.new
    game.moves = %w[d4 d5 c4 dxc4 e4 e5 Nf3 Bb4+ Nc3 exd4 Nxd4 Ne7 Bf4 Bxc3+ bxc3 Ng6 Bg3 Qe7 Bxc4 Qxe4+ Qe2 Qxe2+ Bxe2 Na6 Rb1 e8h8]
    assert_equal '*', game.result
  end

  def test_uci_black_long_castling
    game = Chess::Game.new
    game.moves = %w[e4 c5 Nf3 d6 d4 cxd4 Qxd4 Nc6 Bb5 Bd7 Bxc6 Bxc6 Bg5 Nf6 Bxf6 gxf6 Nc3 e6 e1c1 Be7 Rhe1 Rg8 Qe3 Rxg2 Rg1 Rg6 Nd4 Qb6 h4 e8a8]
    assert_equal '*', game.result
  end

  def test_uci_invalid_white_long_castling
    game = Chess::Game.new
    assert_raises(Chess::IllegalMoveError) do
      game.moves = %w[e4 c5 Nf3 Nc6 d4 cxd4 Nxd4 Nf6 Nc3 d6 Bg5 Qb6 Nb3 e6 Qd2 Be7 e8a8]
    end
  end

  def test_github_issue_31
    g = Chess::Game.load_fen('4Q3/8/8/8/8/8/4K3/7k w - - 0 1')
    assert_equal 'Q', g.board['e8']
    assert_equal 'K', g.board['e2']
    assert_equal 'k', g.board['h1']
    assert_equal '*', g.result
    assert_equal :in_progress, g.status
    g << 'e8a8' # It should not be considered as a UCI casting
    assert_equal 'Q', g.board['a8']
  end
end
