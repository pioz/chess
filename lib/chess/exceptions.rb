module Chess

  # This exception will be raised when an invalid short algebraic chess notation string is passed to the Game#move function.
  class BadNotationError < StandardError
    # :nodoc:
    # Create a new exception.
    def initialize(notation)
      super("The notation '#{notation}' is invalid")
    end
  end

  # This exception will be raised when a malformed PGN file is loaded.
  class InvalidPgnFormatError < StandardError
    # :nodoc:
    # Create a new exception.
    def initialize(filename)
      super("Invalid PGN file: '#{filename}'")
    end
  end

  # This exception will be raised when an invalid FEN string is used.
  class InvalidFenFormatError < StandardError
    # :nodoc:
    # Create a new exception.
    def initialize(fen_string)
      super("Invalid FEN string: '#{fen_string}'")
    end
  end

end

