require 'chess/chess' # this is the compiled ruby extension (chess.so or chess.bundle)
require_relative 'chess/exceptions'
require_relative 'chess/game'
require_relative 'chess/gnuchess'
require_relative 'chess/pgn'
require_relative 'chess/version'

# While development to require 'chess' from irb run with `irb -I lib:ext`
# $ irb -I lib:ext
# irb(main):001:0> require 'chess'
# => true
# irb(main):002:0> Chess::Game.new
# => #<Chess::Game:0x0000000000000001>
