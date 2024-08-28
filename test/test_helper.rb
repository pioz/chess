require 'simplecov'
SimpleCov.start do
  add_filter 'lib/chess/gnuchess.rb'
end
if ENV['CODECOV_TOKEN']
  require 'codecov'
  SimpleCov.formatter = SimpleCov::Formatter::Codecov
end

require 'chess'
require 'debug'
require 'minitest/autorun'

module TestHelper
  PGN_COLLECTION = 'test/pgn_collection'.freeze
  BIG_PGN_COLLECTION = '/Users/pioz/Code/prog/misc/pgn_collection/pgn'.freeze

  def self.pgns(path, prefix = PGN_COLLECTION)
    Dir[File.join(prefix, path, '**/*.pgn')]
  end

  def self.pick_pgn(path, prefix = PGN_COLLECTION)
    file = File.join(prefix, path)
    return Chess::Pgn.new(file)
  end
end
