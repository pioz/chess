# Chess

![build](https://github.com/pioz/chess/workflows/Ruby/badge.svg)
[![codecov](https://codecov.io/gh/pioz/chess/branch/master/graph/badge.svg?token=bsSUOW6wWa)](https://codecov.io/gh/pioz/chess)

A fast Ruby gem to play chess with Ruby. This library is quite fast because manage game situations with [bitboards](https://en.wikipedia.org/wiki/Bitboard). Also, the move generator is written in C as a Ruby extension.

## Requirements

- Ruby 2.5 or higher

## Installation

    gem install chess

## Usage

```ruby
require 'chess'

g = Chess::Game.new
until g.over?
  begin
    print "Give me a #{g.active_player} move: "
    input = gets.chop
    break if input == 'quit'
    g << input
    puts g
    puts g.moves.last
  rescue Chess::IllegalMoveError => e
    puts 'Illegal move!'
  end
end
puts g.status
```

📚 **Documentation** is available [here](https://pioz.github.io/chess).

## Questions or problems?

If you have any issues please add an [issue on
GitHub](https://github.com/pioz/chess/issues) or fork the project and send a
pull request.

## Copyright

Copyright (c) 2024 [Enrico Pilotto (@pioz)](https://github.com/pioz). See
[LICENSE](https://github.com/pioz/chess/blob/master/LICENSE) for details.
