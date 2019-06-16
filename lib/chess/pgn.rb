module Chess
  # Rappresents a game in PGN (Portable Game Notation) format.
  class Pgn
    # Array that include PGN standard tags.
    TAGS = %w[event site date round white black result].freeze

    # The name of the tournament or match event.
    # @return [String]
    attr_accessor :event
    # The location of the event. This is in City, Region COUNTRY format, where
    # COUNTRY is the three-letter International Olympic Committee code for the
    # country.
    # @return [String]
    attr_accessor :site
    # The starting date of the game, in YYYY.MM.DD form. ?? is used for unknown values.
    # @return [String]
    attr_reader :date
    # The playing round ordinal of the game within the event.
    # @return [String]
    attr_accessor :round
    # The player of the white pieces, in Lastname, Firstname format.
    # @return [String]
    attr_accessor :white
    # The player of the black pieces, same format as White.
    # @return [String]
    attr_accessor :black
    # The result of the game. This can only have four possible values: 1-0
    # (White won), 0-1 (Black won), 1/2-1/2 (Draw), or * (other, e.g., the game
    # is ongoing).
    # @return [String]
    attr_accessor :result
    # The array with all moves done in short algebraic chess notation.
    # @return [Array<String>]
    attr_accessor :moves

    # @param [String] filename The path of the PGN file.
    # @param [Boolean] check_moves If true check if the moves are legal.
    # @raise [InvalidPgnFormatError]
    # @raise [IllegalMoveError]
    def initialize(filename = nil, check_moves: false)
      self.load(filename, check_moves: check_moves) if filename
      @date = '??'
      @round = '1'
    end

    # Load a PGN from file.
    # @param [String] filename The path of the PGN file.
    # @param [Boolean] check_moves If true check if the moves are legal.
    # @return [Pgn] Returns `self`.
    # @raise [InvalidPgnFormatError]
    # @raise [IllegalMoveError]
    def load(filename, check_moves: false)
      data = File.open(filename, 'r').read
      data.gsub!(/\{.*?\}/, '') # remove comments
      TAGS.each do |t|
        instance_variable_set("@#{t}", data.match(/^\[#{t.capitalize} ".*"\]\s?$/).to_s.strip[t.size + 3..-3])
      end
      @result = '1/2-1/2' if @result == '1/2'
      game_index = data.index(/^1\./)
      raise Chess::InvalidPgnFormatError.new(filename) if game_index.nil?

      game = data[game_index..-1].strip
      @moves = game.tr("\n", ' ').split(/\d+\./).collect(&:strip)[1..-1].collect { |t| t.split(' ') }.flatten
      @moves.delete_at(@moves.size - 1) if @moves.last =~ /(0-1)|(1-0)|(1\/2)|(1\/2-1\/2)|(\*)/
      @moves.each do |m|
        if m !~ MOVE_REGEXP && m !~ SHORT_CASTLING_REGEXP && m !~ LONG_CASTLING_REGEXP
          raise Chess::InvalidPgnFormatError.new(filename)
        end
      end
      Chess::Game.new(@moves) if check_moves
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
        m << "#{i / 2 + 1}. " if i.even?
        m << "#{move} "
      end
      m << @result unless @result.nil?
      s << m.gsub(/(.{1,78})(?: +|$)\n?|(.{78})/, "\\1\\2\n")
    end

    # Write PGN to file.
    # @param [String] filename The path of the PGN file.
    def write(filename)
      File.open(filename, 'w') { |f| f.write(self.to_s) }
    end

    # # @!visibility private
    # alias old_date= date=

    # Set the date tag.
    def date=(value)
      if value.is_a?(Time)
        @date = value.strftime('%Y.%m.%d')
      else
        @date = value
      end
    end
  end
end
