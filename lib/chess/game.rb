require File.join(File.dirname(__FILE__), '../../ext/chess')

module Chess
  # This class rappresents a chess game.
  class Game < CGame
    # @param [Array<String>] moves If an array of moves is provided, the moves will be performed.
    # @raise [IllegalMoveError]
    # @raise [BadNotationError]
    def initialize(moves = [])
      super()
      moves.each { |m| move(m) }
    end

    # Creates a new game from a file in PGN format.
    # @param [String] file The path of the PGN to laod.
    # @return [Game]
    # @raise [InvalidPgnFormatError]
    # @raise [IllegalMoveError]
    # @raise [BadNotationError]
    def self.load_pgn(file)
      pgn = Chess::Pgn.new(file)
      game = Chess::Game.new
      pgn.moves.each { |m| game.move(m) }
      unless game.over?
        case pgn.result
        when '1-0'
          game.resign(:black)
        when '0-1'
          game.resign(:white)
        when '1/2-1/2'
          game.draw
        end
      end
      return game
    end

    # Creates a new game from a FEN string.
    # @param [String] fen The FEN string to laod.
    # @return [Game]
    # @raise [InvalidFenFormatError]
    # @note This game do not have history before the FEN placement.
    def self.load_fen(fen)
      raise InvalidFenFormatError.new(fen) unless /^((?:[PRNBQKprnbqk1-8]{1,8}\/){7}[RNBQKPrnbqkp1-8]{1,8})\s(w|b)\s(K?Q?k?q?|-)\s([a-h][1-8]|-)\s(\d+)\s(\d+)$/.match?(fen)

      game = Chess::Game.new
      game.set_fen!(fen)
      return game
    end

    # Make a move.
    # @note This add a new {Board} in the {Game}.
    # @param [String] notation Represents the short algebraic chess notation string of
    #   the move. `notation` can be also _from_square_ plus _to_square_ _('e2e4', ...,
    #   'b1c3')_ (coordinate chess notation).
    # @return [String] Returns a string that represents the short algebraic
    #   chess notation of the move.
    #
    # @raise {IllegalMoveError} if the move is illegal.
    # @raise {BadNotationError} if the short algebraic chess notation is
    #   malformed.
    def move(notation)
      expand = expand_move(notation)
      if expand[:from]
        move2(expand[:from], expand[:to], expand[:promotion])
      else
        super(expand[:name], expand[:dis], expand[:to], expand[:promotion])
      end
    rescue IllegalMoveError
      raise IllegalMoveError.new("Illegal move '#{notation}'\nStatus: #{self.status}\nPlayer turn #{self.active_player}\n#{self}") if ENV['DEBUG']

      raise IllegalMoveError.new("Illegal move '#{notation}'")
    end
    alias move= move
    alias << move

    # Make the array of moves.
    # @param [Array<String>] moves The array of moves to performe.
    def moves=(moves)
      moves.each { |m| move(m) }
    end

    # Returns `:white` if the active player is the white player, `:black`
    # otherwise.
    # @return [Symbol]
    def active_player
      self.board.active_color ? :black : :white
    end

    # Returns `:white` if the inactive player is the white player, `:black`
    # otherwise.
    # @return [Symbol]
    def inactive_player
      self.board.active_color ? :white : :black
    end

    # Returns the status of the game.
    # Possible states are:
    #
    # * `in_progress`: the game is in progress.
    # * `white_won`: white player has won with a checkmate.
    # * `black_won`: black player has won with a checkmate.
    # * `white_won_resign`: white player has won for resign.
    # * `black_won_resign`: black player has won for resign.
    # * `stalemate`: draw for stalemate.
    # * `insufficient_material`: draw for insufficient material to checkmate.
    # * `fifty_move_rule`: draw for fifty-move rule.
    # * `threefold_repetition`: draw for threefold repetition.
    # @return [Symbol]
    def status
      case self.result
      when '*'
        return :in_progress
      when '1-0'
        return :white_won if self.board.checkmate?

        return :white_won_resign
      when '0-1'
        return :black_won if self.board.checkmate?

        return :black_won_resign
      when '1/2-1/2'
        return :stalemate if self.board.stalemate?
        return :insufficient_material if self.board.insufficient_material?
        return :fifty_move_rule if self.board.fifty_move_rule?

        return :threefold_repetition if self.threefold_repetition?
      end
    end

    # Returns `true` if the game is over.
    def over?
      return self.result != '*'
    end

    # Returns the PGN rappresenting the game.
    # @return [Chess::Pgn]
    def pgn
      pgn = Chess::Pgn.new
      pgn.moves = self.moves
      pgn.result = self.result
      return pgn
    end

    private

    # Expand the short algebraic chess notation string `notation` in a hash like this:
    #
    #     Ngxe2 ==> { name: 'N', dis: 'g', from: nil, to: 'e2', promotion: nil }
    def expand_move(notation)
      if (match = notation.match(MOVE_REGEXP))
        expand = {
          name: match[1] || 'P',    # Piece name [RNBQK]
          dis: match[2],            # Disambiguating move
          to: match[3],             # Move to
          promotion: match[4]       # Promote with
        }
        expand[:from] = match[2] if match[2] && match[2].size == 2
        return expand
      elsif SHORT_CASTLING_REGEXP.match?(notation)
        return { name: 'K', dis: nil, from: 'e8', to: 'g8', promotion: nil } if self.board.active_color # black king short castling

        return { name: 'K', dis: nil, from: 'e1', to: 'g1', promotion: nil } # white king short castling
      elsif LONG_CASTLING_REGEXP.match?(notation)
        return { name: 'K', dis: nil, from: 'e8', to: 'c8', promotion: nil } if self.board.active_color # black king long castling

        return { name: 'K', dis: nil, from: 'e1', to: 'c1', promotion: nil } # white king long castling
      end
      raise BadNotationError.new(notation)
    end
  end

  MOVE_REGEXP = /^([RNBQK])?([a-h]|[1-8]|[a-h][1-8])?(?:x)?([a-h][1-8])(?:=?([RrNnBbQq]))?(?:ep)?(?:\+|\#)?$/.freeze
  SHORT_CASTLING_REGEXP = /^([0O])-([0O])([+#])?$/.freeze
  LONG_CASTLING_REGEXP = /^([0O])-([0O])-([0O])([+#])?$/.freeze
  private_constant :MOVE_REGEXP
  private_constant :SHORT_CASTLING_REGEXP
  private_constant :LONG_CASTLING_REGEXP
end
