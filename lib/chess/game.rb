require File.join(File.dirname(__FILE__), '../../ext/chess')

module Chess

  # This class rappresents a chess game.
  class Game < CGame

    # Create a new game. If an array of moves is provided, the moves will be performed.
    # May be raise an IllegalMoveError or BadNotationError.
    def initialize(moves = [])
      moves.each { |m| move(m) }
    end

    # Creates a new game from a file in PGN format.
    # May be raise an InvalidPgnFormatError or IllegalMoveError or BadNotationError.
    def self.load_from_pgn(file)
      pgn = Chess::Pgn.new(file)
      game = Chess::Game.new
      pgn.moves.each { |m| game.move(m) }
      return game
    end

    # Make a move. This add a new Board in the Storyboard.
    # The parameter +m+ represents the short algebraic chess notation string of the move.
    # +m+ can be <em>from_square</em> plus <em>to_square</em> <em>('e2e4', ..., 'b1c3')</em>.
    # This method returns a string that represents the short algebraic chess notation of the move.
    # Raise an IllegalMoveError if the move is illegal.
    # Raise an BadNotationError if the short algebraic chess notation is malformed.
    def move(m)
      begin
        expand = expand_move(m)
        if expand[:from]
          move2(expand[:from], expand[:to], expand[:promotion])
        else
          super(expand[:name], expand[:dis], expand[:to], expand[:promotion])
        end
      rescue IllegalMoveError => e
        raise IllegalMoveError.new("Illegal move '#{m}'\n#{self.to_s}")
      end
    end
    alias :move= :move

    # Make the array of moves.
    def moves=(moves)
      moves.each { |m| move(m) }
    end

    # Return `:white` if the active player is the white player, `:black` otherwise.
    def active_player
      self.board.active_color ? :black : :white
    end

    # Return `:white` if the inactive player is the white player, `:black` otherwise.
    def inactive_player
      self.board.active_color ? :white : :black
    end

    private

    # Expand the short algebraic chess notation string +m+ in a hash like this:
    #     Ngxe2 ==> { :name => 'N', :dis => 'g', :from => nil, :to => 'e2', :promotion => '' }
    def expand_move(m)
      if match = m.match(/^(R|N|B|Q|K)?([a-h]?[1-8]?)(?:x)?([a-h][1-8])(?:=?(R|N|B|Q))?(?:ep)?(?:\+|\#)?$/)
        expand = {
          :name => match[1] || 'P',    # Piece name (P|R|N|B|Q|K)
          :dis => match[2],            # Disambiguating move
          :to => match[3],             # Move to
          :promotion => match[4].to_s, # Promote with
        }
        expand[:from] = match[2] if match[2] && match[2].size == 2
        return expand
      elsif m =~ /^(0|O)-(0|O)(\+|\#)?$/
        if self.board.active_color # black king short castling
          return { :name => 'K', :dis => '', :from => 'e8', :to => 'g8', :promotion => '' }
        else # white king short castling
          return { :name => 'K', :dis => '', :from => 'e1', :to => 'g1', :promotion => '' }
        end
      elsif m =~ /^(0|O)-(0|O)-(0|O)(\+|\#)?$/
        if self.board.active_color # black king long castling
          return { :name => 'K', :dis => '', :from => 'e8', :to => 'c8', :promotion => '' }
        else # white king long castling
          return { :name => 'K', :dis => '', :from => 'e1', :to => 'c1', :promotion => '' }
        end
      end
      raise BadNotationError.new(m)
    end

  end
end