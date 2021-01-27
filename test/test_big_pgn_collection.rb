require 'test_helper'

class ChessTest < Minitest::Test
  if ENV['BIG_PGN_COLLECTION'] && File.exist?(TestHelper::BIG_PGN_COLLECTION)
    puts 'Loading tests for a big PGN collection...'
    300_000.times do |i|
      filename = format('%06d.pgn', i + 1)
      path = File.join(TestHelper::BIG_PGN_COLLECTION, filename)
      break unless File.exist?(path)

      define_method "test_big_pgn_#{filename}" do
        pgn = Chess::Pgn.new(path)
        game = Chess::Game.new(pgn.moves)
        assert(game.checkmate?) if pgn.moves.last.match?(/\#$/)
      end
    end
  end
end
