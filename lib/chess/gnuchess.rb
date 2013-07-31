module Chess

  # Use Gnuchess to I.A. <em>(Only a draft)</em>
  # To use this module, extend a game object with Chess::Gnuchess.
  # Gnuchess binary have to be installed.
  #
  #    g = Chess::Game.new
  #    g.extend Chess::Gnuchess
  #    g.gnuchess_move!
  #    puts g
  module Gnuchess

    # Return the next move calculated by Gnuchess.
    # Gnuchess must be installed!
    def gnuchess_move
      pipe = IO.popen('gnuchess -x', 'r+')
      begin
        pipe.puts('depth 1')
        pipe.puts('manual')
        self.full_moves.each do |m|
          pipe.puts(m)
        end
        pipe.puts('go')
        while line = pipe.gets
          raise IllegalMoveError if line.include?('Invalid move')
          match = line.match(/My move is : ([a-h][1-8][a-h][1-8][rkbq]?)/)
          return match[1] if match
        end
      ensure
        pipe.puts('quit')
        pipe.close
      end
      return moves
    end

    # Make a move using Gnuchess engine. This add a new Board in the Game.
    # Return the next move calculated by Gnuchess.
    # Gnuchess must be installed!
    def gnuchess_move!
      next_move = self.gnuchess_move
      self.move(next_move) if next_move
    end

    # :nodoc:
    def self.included(mod)
      raise_if_gnuchess_is_not_installed
    end

    # :nodoc:
    def self.extended(mod)
      raise_if_gnuchess_is_not_installed
    end

    # :nodoc:
    def gen_pgn(file_to_save, moves = [])
      done = false
      pipe = IO.popen('gnuchess', 'r+')
      begin
        pipe.write("depth 1\n")
        pipe.write("force\n")
        pipe.write("name chess.rb\n")
        moves.each do |move|
          pipe.write("#{move}\n")
        end
        while(!done)
          pipe.write("go\n")
          while line = pipe.gets
            if line =~ /My move is : /
              break
            elsif line =~ / : resign/
              break
            elsif line =~ / : 1-0 {White mates}/
              done = :white_won
              break
            elsif line =~ / : 0-1 {Black mates}/
              done = :black_won
              break
            elsif m = line.match(/1\/2-1\/2 {(.*?)}/)
              case m[1]
              when 'Stalemate'
                done = :stalemate
              when 'Draw by repetition'
                done = :repetition
              when 'Draw by fifty-move rule'
                done = :fifty_move_rule
              when 'Draw by insufficient material'
                done = :insufficient_material
              end
              break
            end
          end
        end
        dir = File.dirname(file_to_save)
        name = File.basename(file_to_save)
        pipe.write("pgnsave #{File.join(dir, done.to_s, name)}\n")
      ensure
        pipe.write("quit\n")
        pipe.close
      end
    end

    private

    # Return true if Gnuchess is installed, false otherwise.
    def self.gnuchess_installed?
      system('which gnuchess >& /dev/null')
      $?.exitstatus == 0
    end

    # Raise an exception if Gnuchess is not installed
    def self.raise_if_gnuchess_is_not_installed
      unless gnuchess_installed?
        raise 'You need to install Gnuchess to use the module Chess::Gnuchess.'
      end
    end

  end

end

