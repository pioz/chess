require 'test_helper'

class ChessTest < Minitest::Test
  GENS = {
    'r2qk3/8/2n5/8/8/8/p2B4/4K2R w K - 0 1' => { 'e1' => %w[Kd1 Ke2 Kf2 Kf1 O-O] },
    'r2qk3/8/2n5/8/8/8/p2B4/4K2R w - - 0 1' => { 'e1' => %w[Kd1 Ke2 Kf2 Kf1] },
    'r2qk3/8/2n5/2b5/8/8/p2B4/4K2R w K - 0 1' => { 'e1' => %w[Kd1 Ke2 Kf1] },
    'r2qk3/8/2n5/2b5/8/8/p2B4/4K2R b K - 0 1' => { 'a2' => ['a1=Q'] },
    'r2qk3/8/2n1n3/2b5/8/8/p2B4/4K2R b K - 0 1' => { 'e6' => %w[Nf8 Ng7 Ng5 Nf4 Ned4 Nc7] },
    'r2qk3/8/2n5/2b2pP1/8/8/3B4/4K2R w K f6 0 1' => { 'g5' => %w[gxf6ep g6] },
    '1B1k4/r3q3/2n1n3/2b2pP1/8/1n6/3B4/4K2R b K - 0 1' => { 'c6' => %w[Nxb8 Ne5 Ncd4 Nb4 Nca5] },
    '1B1k4/r3q3/2n1n3/2b2pP1/8/8/2nB4/3K3R b - - 0 1' => { 'c6' => %w[Nxb8 Ne5 Nc6d4 N6b4 Na5] },
    '1B1k4/r3q3/2n1n2P/2b5/5p2/3p1RP1/3B4/3K4 w - - 0 1' => { 'f3' => %w[Rxd3 Re3 Rxf4 Rf2 Rf1] },
    'k7/1Q6/2P5/8/8/8/8/3K4 b - - 0 1' => { 'a8' => [] },
    'k7/6P1/8/8/8/3r4/3Q4/3K4 w - - 0 1' => { 'd2' => ['Qxd3'] },
    'rnbqkbnr/1ppppppp/8/8/pP2P3/P7/2PP1PPP/RNBQKBNR w KQkq - 0 3' => { 'c2' => %w[c3 c4] }
  }.freeze

  GENS.each do |fen, generators|
    define_method :"test_move_generator_#{fen}" do
      game = Chess::Game.load_fen(fen)
      generators.each do |from, moves|
        result = game.board.generate_moves(from)

        assert_equal moves.sort, result.sort
      end
    end
  end
end
