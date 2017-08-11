require File.join(File.dirname(__FILE__), '../../ext/chess')

module Chess

  # This class rappresents a chess game.
  class Game < CGame

    # Create a new game. If an array of moves is provided, the moves will be performed.
    #
    # May be raise an IllegalMoveError or BadNotationError.
    def initialize(moves = [])
      moves.each { |m| move(m) }
    end

    # Creates a new game from a file in PGN format.
    #
    # May be raise an InvalidPgnFormatError or IllegalMoveError or BadNotationError.
    def self.load_pgn(file)
      pgn = Chess::Pgn.new(file)
      game = Chess::Game.new
      pgn.moves.each { |m| game.move(m) }
      if !game.over?
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
    #
    # May be raise an InvalidFenFormatError.
    #
    # *Warning*: this game do not have history before the FEN placement.
    def self.load_fen(fen)
      if fen =~ /^((?:[PRNBQKprnbqk1-8]{1,8}\/){7}[RNBQKPrnbqkp1-8]{1,8})\s(w|b)\s(K?Q?k?q?|-)\s([a-h][1-8]|-)\s(\d+)\s(\d+)$/
        game = Chess::Game.new
        game.set_fen!(fen)
        return game
      else
        raise InvalidFenFormatError.new(fen)
      end
    end

    # Make a move. This add a new Board in the Storyboard.
    #
    # Parameters are:
    # +m+:: represents the short algebraic chess notation string of the move.
    #       +m+ can be <em>from_square</em> plus <em>to_square</em>
    #       <em>('e2e4', ..., 'b1c3')</em> (coordinate chess notation).
    #
    # This method returns a string that represents the short algebraic chess
    # notation of the move.
    #
    # Raise an IllegalMoveError if the move is illegal.
    # Raise an BadNotationError if the short algebraic chess notation is
    # malformed.
    def move(m)
      begin
        expand = expand_move(m)
        if expand[:from]
          move2(expand[:from], expand[:to], expand[:promotion])
        else
          super(expand[:name], expand[:dis], expand[:to], expand[:promotion])
        end
      rescue IllegalMoveError
        if ENV['DEBUG']
          raise IllegalMoveError.new("Illegal move '#{m}'\nStatus: #{self.status}\nPlayer turn #{self.active_player}\n#{self.to_s}")
        else
          raise IllegalMoveError.new("Illegal move '#{m}'")
        end
      end
    end
    alias :move= :move
    alias :<< :move

    # Make the array of moves.
    def moves=(moves)
      moves.each { |m| move(m) }
    end

    # Returns +:white+ if the active player is the white player, +:black+ otherwise.
    def active_player
      self.board.active_color ? :black : :white
    end

    # Returns +:white+ if the inactive player is the white player, +:black+ otherwise.
    def inactive_player
      self.board.active_color ? :white : :black
    end

    # Returns the status of the game.
    #
    # Possible states are:
    # * +in_progress+:: the game is in progress.
    # * +white_won+:: white player has won with a checkmate.
    # * +black_won+:: black player has won with a checkmate.
    # * +white_won_resign+:: white player has won for resign.
    # * +black_won_resign+:: black player has won for resign.
    # * +stalemate+:: draw for stalemate.
    # * +insufficient_material+:: draw for insufficient material to checkmate.
    # * +fifty_rule_move+:: draw for fifty rule move.
    # * +threefold_repetition+:: draw for threefold_repetition.
    # * +unknown+:: something went wrong.
    def status
      case self.result
      when '*'
        return :in_progress
      when '1-0'
        if self.board.checkmate?
          return :white_won
        else
          return :white_won_resign
        end
      when '0-1'
        if self.board.checkmate?
          return :black_won
        else
          return :black_won_resign
        end
      when '1/2-1/2'
        if self.board.stalemate?
          return :stalemate
        elsif self.board.insufficient_material?
          return :insufficient_material
        elsif self.board.fifty_rule_move?
          return :fifty_rule_move
        elsif self.threefold_repetition?
          return :threefold_repetition
        end
      end
      return :unknown
    end

    # Returns +true+ if the game is over
    def over?
      return self.result != '*'
    end

    # Returns the PGN rappresenting the game.
    def pgn
      pgn = Chess::Pgn.new
      pgn.moves = self.moves
      pgn.result = self.result
      return pgn
    end

    private

    # Expand the short algebraic chess notation string +m+ in a hash like this:
    #
    #     Ngxe2 ==> { name: 'N', dis: 'g', from: nil, to: 'e2', promotion: nil }
    def expand_move(m)
      if match = m.match(/^([RNBQK])?([a-h]|[1-8]|[a-h][1-8])?(?:x)?([a-h][1-8])(?:=?([RrNnBbQq]))?(?:ep)?(?:\+|\#)?$/)
        expand = {
          name: match[1] || 'P',    # Piece name [RNBQK]
          dis: match[2],            # Disambiguating move
          to: match[3],             # Move to
          promotion: match[4],      # Promote with
        }
        expand[:from] = match[2] if match[2] && match[2].size == 2
        return expand
      elsif m =~ /^([0O])-([0O])([\+#])?$/
        if self.board.active_color # black king short castling
          return { name: 'K', dis: nil, from: 'e8', to: 'g8', promotion: nil }
        else # white king short castling
          return { name: 'K', dis: nil, from: 'e1', to: 'g1', promotion: nil }
        end
      elsif m =~ /^([0O])-([0O])-([0O])([\+#])?$/
        if self.board.active_color # black king long castling
          return { name: 'K', dis: nil, from: 'e8', to: 'c8', promotion: nil }
        else # white king long castling
          return { name: 'K', dis: nil, from: 'e1', to: 'c1', promotion: nil }
        end
      end
      raise BadNotationError.new(m)
    end

  end
end
