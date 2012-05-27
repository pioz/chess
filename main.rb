$LOAD_PATH << './lib'
require 'chess'

g = Chess::Game.new

g.extend Chess::Gnuchess

# 1000.times do |i|
#   puts g.gnuchess_move
# end

while (1)
  begin
    print 'Give me a move: '
    white_move = gets.chop
    g.move = white_move
    puts g.board
    puts g.board.to_fen

    g.gnuchess_move
    puts "Gnuchess move is #{g.moves.last}"
    puts g.board
    puts g.board.to_fen
  rescue Chess::IllegalMoveError => e
    puts 'Illegal move!'
  end
end
