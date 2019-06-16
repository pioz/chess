require 'test_helper'

class ChessTest < Minitest::Test
  TestHelper.pgns('invalid').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_invalid_pgn_#{name}" do
      assert_raises(Chess::InvalidPgnFormatError) do
        Chess::Pgn.new(file)
      end
    end
  end

  TestHelper.pgns('illegal').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_illegal_pgn_#{name}" do
      assert_raises(Chess::IllegalMoveError) do
        Chess::Pgn.new(file, check_moves: true)
      end
      Chess::Pgn.new(file)
    end
  end

  def test_load_from_string
    pgn_string = <<~PGN
      [Event "70th ch-ITA"]
      [Site "Siena ITA"]
      [Round "10"]
      [Date "2010.12.3"]
      [White "Caruana, Fabiano"]
      [Black "Godena, Michele"]
      [Result "1-0"]
      1.d4 d5 2.c4 dxc4 3.e4 e5 4.Nf3 Bb4+ 5.Nc3 exd4 6.Nxd4 Ne7 7.Bf4 Bxc3+ 8.bxc3 Ng6 9.Bg3 Qe7 10.Bxc4 Qxe4+ 11.Qe2 Qxe2+ 12.Bxe2 Na6 13.Rb1 O-O 14.O-O Re8 15.Rfe1 Nc5 16.Bxc7 Bd7 17.Bf3 Rxe1+ 18.Rxe1 Rc8 19.Bg3 b6 20.h4 Ne6 21.h5 Ne7 22.Be5 Nc6 23.Nxc6 Bxc6 24.Bg4 Re8 25.Bg3 g6 26.h6 f5 27.Bd1 f4 28.Bh4 Kf8 29.Re5 g5 30.Bh5 Rc8 31.Bxg5 Nxg5 32.Rxg5 Bd7 33.Rg7 Rc5 34.Bf3 Bf5 35.Rxa7 Rxc3 36.Bd5 Bg6 37.Ra4 Rc1+ 38.Kh2 Rc5 39.Rxf4+ Ke7 40.Bf3 Ra5 41.Rb4 b5 42.Bd5 Kf6 43.f4 Bf5 44.Bc6 Bd3 45.Rd4 Ra3 46.Bd5 Bb1 47.Rd1 Bd3 48.Bb3 Bc4 49.Bc2 Ke7 50.Bf5 Rxa2 51.Rd7+ Kf8 52.Rxh7 Bd5 53.Rd7 1-0
    PGN
    pgn = Chess::Pgn.new
    pgn.load_from_string(pgn_string, check_moves: true)
    assert_equal '70th ch-ITA', pgn.event
    assert_equal 'Siena ITA', pgn.site
    assert_equal '10', pgn.round
    assert_equal '2010.12.3', pgn.date
    assert_equal 'Caruana, Fabiano', pgn.white
    assert_equal 'Godena, Michele', pgn.black
    assert_equal 'Godena, Michele', pgn.black
    assert_equal '1-0', pgn.result
    assert_equal 'Rd7', pgn.moves.last
  end

  def test_load_from_string_without_tags
    pgn_string = <<~PGN
      1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 {This opening is called the Ruy Lopez.}
      4. Ba4 Nf6 5. O-O Be7 6. Re1 b5 7. Bb3 d6 8. c3 O-O 9. h3 Nb8 10. d4 Nbd7
      11. c4 c6 12. cxb5 axb5 13. Nc3 Bb7 14. Bg5 b4 15. Nb1 h6 16. Bh4 c5 17. dxe5
      Nxe4 18. Bxe7 Qxe7 19. exd6 Qf6 20. Nbd2 Nxd6 21. Nc4 Nxc4 22. Bxc4 Nb6
      23. Ne5 Rae8 24. Bxf7+ Rxf7 25. Nxf7 Rxe1+ 26. Qxe1 Kxf7 27. Qe3 Qg5 28. Qxg5
      hxg5 29. b3 Ke6 30. a3 Kd6 31. axb4 cxb4 32. Ra5 Nd5 33. f3 Bc8 34. Kf2 Bf5
      35. Ra7 g6 36. Ra6+ Kc5 37. Ke1 Nf4 38. g3 Nxh3 39. Kd2 Kb5 40. Rd6 Kc5 41. Ra6
      Nf2 42. g4 Bd3 43. Re6 1/2-1/2
    PGN
    pgn = Chess::Pgn.new
    pgn.load_from_string(pgn_string, check_moves: true)
    assert_nil pgn.event
    assert_nil pgn.site
    assert_nil pgn.round
    assert_nil pgn.date
    assert_nil pgn.white
    assert_nil pgn.black
    assert_nil pgn.black
    assert_nil pgn.result
    assert_equal 'Re6', pgn.moves.last
  end
end
