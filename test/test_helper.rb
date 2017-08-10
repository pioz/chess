require 'chess'
require 'minitest/autorun'

module TestHelper

  PGN_COLLECTION = 'test/pgn_collection'
  BIG_PGN_COLLECTION = '/Users/pioz/Desktop/pgn_collection/pgn'

  def self.pgns(path, prefix = PGN_COLLECTION)
    Dir[File.join(prefix, path, '**/*.pgn')]
  end

end
