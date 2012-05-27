module Chess

  # Rappresents a game in PGN (Portable Game Notation) format.
  class Pgn

    # Array that include PGN standard tags.
    TAGS = ['event', 'site', 'date', 'round', 'white', 'black', 'result']

    attr_accessor *(TAGS + [:moves])

    # Creates a new PGN. If param +filename+, load it from file.
    def initialize(filename = nil)
      load(filename) if filename
    end

    # Load a PGN from file.
    def load(filename)
      data = File.open(filename, 'r').read
      data.gsub!(/\{.*?\}/, '')
      TAGS.each do |t|
        instance_variable_set("@#{t}", data.match(/^\[#{t.capitalize} ".*"\]\s?$/).to_s.strip[t.size+3..-3])
      end
      game_index = data.index(/^1\./)
      raise Chess::InvalidPgnFormatError.new(filename) if game_index.nil?
      game = data[game_index..-1].strip
      @moves = game.gsub("\n", ' ').split(/\d+\./).collect{|t| t.strip}[1..-1].collect{|t| t.split(' ')}.flatten
      @moves.delete_at(@moves.size-1) if @moves.last =~ /(0\-1)|(1\-0)|(1\/2)|(1\/2\-1\/2)|(\*)/
      return self
    end

    # PGN to string.
    def to_s
      s = ''
      TAGS.each do |t|
        s << "[#{t.capitalize} \"#{instance_variable_get("@#{t}")}\"]\n" unless instance_variable_get("@#{t}").nil?
      end
      s << "\n"
      @moves.each_with_index do |m, i|
        if i % 2 == 0
          s << "#{i+1}. #{m}"
        else
          s << " #{m} "
        end
      end
      s << @result unless @result.nil?
    end

    # Write PGN to file.
    def write(filename)
      File.open(filename, 'w') { |f| f.write(self.to_s) }
    end

  end

end
