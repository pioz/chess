require 'test_helper'

class ChessTest < Minitest::Test
  def test_moves
    game = Chess::Game.new
    game.moves = %w[e4 e5]
    assert_equal %w[e4 e5], game.moves
  end

  def test_active_player
    game = Chess::Game.new
    game << 'a3'
    assert_equal :black, game.active_player
  end

  def test_inactive_player
    game = Chess::Game.new
    game.moves = %w[a3 f5]
    assert_equal :black, game.inactive_player
  end

  def test_status_white_won_resign
    game = Chess::Game.new
    game.resign(:black)
    assert_equal :white_won_resign, game.status
    assert game.over?
    assert_equal '1-0', game.result
  end

  def test_status_black_won_resign
    game = Chess::Game.new
    game << 'e4'
    game.resign(:white)
    assert_equal :black_won_resign, game.status
    assert game.over?
    assert_equal '0-1', game.result
  end

  def test_status_insufficient_material
    pgn = TestHelper.pick_pgn('insufficient_material/0001.pgn')
    game = Chess::Game.new(pgn.moves)
    assert_equal :insufficient_material, game.status
  end

  def test_status_fifty_move_rule
    pgn = TestHelper.pick_pgn('fifty_move_rule/0001.pgn')
    game = Chess::Game.new(pgn.moves)
    game.draw
    assert_equal :fifty_move_rule, game.status
  end

  def test_status_threefold_repetition
    pgn = TestHelper.pick_pgn('threefold_repetition/0001.pgn')
    game = Chess::Game.new(pgn.moves)
    game.draw
    assert_equal :threefold_repetition, game.status
  end

  def test_pgn
    pgn = TestHelper.pick_pgn('valid/0001.pgn')
    game = Chess::Game.new(pgn.moves)
    expected_pgn = <<~PGN
      [Event ""]
      [Site ""]
      [Date "??"]
      [Round ""]
      [White ""]
      [Black ""]
      [Result "*"]

      1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Qxd4 Nc6 5. Bb5 Bd7 6. Bxc6 Bxc6 7. Bg5 Nf6
      8. Bxf6 gxf6 9. Nc3 e6 10. O-O-O Be7 11. Rhe1 Rg8 12. Qe3 Rxg2 13. Rg1 Rg6 14.
      Nd4 Qb6 15. h4 O-O-O 16. h5 Rg5 17. Nd5 Bxd5 18. exd5 e5 19. Qh3+ Kb8 20. Nf5
      Bf8 21. Rxg5 fxg5 22. Ne3 Qb4 23. c4 g4 24. Qxg4 Bh6 25. Kb1 Rc8 26. Rc1 Qd2
      27. Qf5 Bf4 28. Qc2 Qxc2+ 29. Rxc2 Rg8 30. b4 Rg5 31. c5 e4 32. c6 Rxh5 33.
      Rc4 f5 34. b5 Rg5 35. Rc3 Be5 36. Rc1 Bd4 37. Nc4 Bc5 38. Ne5 dxe5 39. Rxc5
      Rg6 40. a4 Rd6 41. a5 b6 42. axb6 axb6 43. Rc2 Rxd5 44. Rb2 h5 45. Ka2 h4 46.
      Ka3 h3 47. Ka4 Rd1 *
    PGN
    assert_equal expected_pgn, game.pgn.to_s
  end
end
