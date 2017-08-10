module Chess

  # Rappresents a game in PGN (Portable Game Notation) format.
  class Pgn

    # Array that include PGN standard tags.
    TAGS = %w(event site date round white black result)

    attr_accessor(*(TAGS + [:moves]))

    # Creates a new PGN. If param +filename+, load it from file.
    def initialize(filename = nil)
      self.load(filename) if filename
      @date = '??'
      @round = '1'
    end

    # Load a PGN from file.
    def load(filename)
      data = File.open(filename, 'r').read
      data.gsub!(/\{.*?\}/, '') # remove comments
      TAGS.each do |t|
        instance_variable_set("@#{t}", data.match(/^\[#{t.capitalize} ".*"\]\s?$/).to_s.strip[t.size+3..-3])
      end
      @result = '1/2-1/2' if @result == '1/2'
      game_index = data.index(/^1\./)
      raise Chess::InvalidPgnFormatError.new(filename) if game_index.nil?
      game = data[game_index..-1].strip
      @moves = game.gsub("\n", ' ').split(/\d+\./).collect{|t| t.strip}[1..-1].collect{|t| t.split(' ')}.flatten
      @moves.delete_at(@moves.size-1) if @moves.last =~ /(0-1)|(1-0)|(1\/2)|(1\/2-1\/2)|(\*)/
      return self
    end

    # PGN to string.
    def to_s
      s = ''
      TAGS.each do |t|
        tag = instance_variable_get("@#{t}")
        s << "[#{t.capitalize} \"#{tag}\"]\n"
      end
      s << "\n"
      m = ''
      @moves.each_with_index do |move, i|
        m << "#{i/2+1}. " if i % 2 == 0
        m << "#{move} "
      end
      m << @result unless @result.nil?
      s << m.gsub(/(.{1,78})(?: +|$)\n?|(.{78})/, "\\1\\2\n")
    end

    # Write PGN to file.
    def write(filename)
      File.open(filename, 'w') { |f| f.write(self.to_s) }
    end

    # :nodoc:
    alias :old_date= :date=

    # :nodoc:
    # Set the date tag.
    def date=(value)
      if value.is_a?(Time)
        @date = value.strftime('%Y.%m.%d')
      else
        @data = value
      end
    end


  end
end
