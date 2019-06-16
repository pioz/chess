module Chess
  # This exception will be raised when an invalid short algebraic chess notation
  # string is passed to the {Game#move} function.
  class BadNotationError < StandardError
    # @param [String] notation The invalid notation.
    def initialize(notation)
      super("Invalid notation '#{notation}'")
    end
  end

  # This exception will be raised when a malformed PGN file is loaded.
  class InvalidPgnFormatError < StandardError
    # @param [String] filename The PGN filename
    def initialize(filename)
      super("Invalid PGN file '#{filename}'")
    end
  end

  # This exception will be raised when an invalid FEN string is used.
  class InvalidFenFormatError < StandardError
    # @param [String] fen_string The FEN string.
    def initialize(fen_string)
      super("Invalid FEN string '#{fen_string}'")
    end
  end
end
