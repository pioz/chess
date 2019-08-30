require 'chess'
require 'minitest/autorun'

module TestHelper
  PGN_COLLECTION = 'test/pgn_collection'.freeze
  BIG_PGN_COLLECTION = '/Users/pioz/Code/prog/misc/pgn_collection/pgn'.freeze

  def self.pgns(path, prefix = PGN_COLLECTION)
    Dir[File.join(prefix, path, '**/*.pgn')]
  end
end
