module Chess

  # Use Gnuchess to I.A. <em>(Only a draft)</em>
  # To use this module, extend a game object with Chess::Gnuchess
  #
  #    g = Chess::Game.new
  #    g.extend Chess::Gnuchess
  #    g.gnuchess_move
  #    puts g
  module Gnuchess

    # Make a move using Gnuchess engine. This add a new Board in the Game.
    # Return the next move calculated by Gnuchess.
    # Gnuchess must be installed!
    def gnuchess_move
      pipe = IO.popen('gnuchess -x', 'r+')
      begin
        pipe.write("depth 1\n")
        pipe.write("force\n")
        self.full_moves.each do |m|
          pipe.write(m + "\n")
        end
        pipe.write("go\n")
        while line = pipe.gets
          raise IllegalMoveError if line.include?('Invalid move')
          match = line.match(/My move is : ([a-h][1-8][a-h][1-8])/)
          return self.move(match[1]) if match
        end
      ensure
        pipe.write("quit\n")
        pipe.close
      end
    end

    # # Open a pipe with Gnuchess.
    # def initialize
    #   @pipe = IO.popen('gnuchess -x', 'r+')
    #   @pipe.write("depth 1\n")
    #   @pipe.write("force\n")
    # end
    #
    # # Return the next move calculated by Gnuchess.
    # def get
    #   @pipe.write("go\n")
    #   match = nil
    #   while !match
    #     match = @pipe.gets.match(/My move is : ([a-h][1-8][a-h][1-8])/)
    #   end
    #   return match[1]
    # end
    #
    # # Make +moves+ on Gnuchess.
    # # Raise an IllegalMoveError if a move in +moves+ is illegal.
    # def set(*moves)
    #   moves.each do |m|
    #     @pipe.write(m + "\n")
    #     match = nil
    #     while !match
    #       line = @pipe.gets.strip
    #       raise Chess::IllegalMoveError if line.include?('Invalid move')
    #       match = line.match(/\d+\.\s#{m}/)
    #     end
    #   end
    # end
    #
    # # Close pipe with Gnuchess.
    # def close
    #   @pipe.close
    # end

    private

    # Return true if Gnuchess is installed, false otherwise.
    def self.gnuchess_installed?
      system('which gnuchess >& /dev/null')
      $?.exitstatus == 0
    end
  end

end

