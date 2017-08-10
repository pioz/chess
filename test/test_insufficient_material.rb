require 'test_helper'

class ChessTest < Minitest::Test
  FENS = [
    '8/k7/8/7B/6b1/7B/6b1/4K3 w - - 0 1',
    '8/2k5/8/1B6/4b3/8/2K5/5B2 w - - 0 1',
    '8/2k5/8/3b4/B7/8/2K5/8 w - - 0 1',
    '8/2k5/8/8/4B3/8/2K5/8 w - - 0 1',
    '8/2k5/8/5b2/8/8/2K5/8 w - - 0 1'
  ]

  FENS.each_with_index do |fen, i|
    define_method("test_insufficient_material_by_fen_#{i}") do
      game = Chess::Game.load_fen(fen)
      assert(game.board.insufficient_material?)
    end
  end

  TestHelper.pgns('insufficient_material').each do |file|
    name = File.basename(file, '.pgn')
    define_method "test_insufficient_material_#{name}" do
      pgn = Chess::Pgn.new(file)
      game = Chess::Game.new(pgn.moves)
      assert(game.board.insufficient_material?)
    end
  end

end
