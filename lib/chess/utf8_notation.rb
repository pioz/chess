module Chess
  # rubocop:disable Style/AsciiComments

  # With this module is possible call the method {#to_utf8} on a string. This
  # method convert the chess piece identifier character into UTF8 chess
  # character.
  # @example
  #   :001 > require 'chess/utf8_notation'
  #   => true
  #   :002 > 'Qf7#'.to_utf8
  #   => '♕f7#'
  #
  # @note To use this utility explicit require is needed: <tt>require
  #   'chess/utf8_notation'</tt>
  module UTF8Notation
    # Map a piece identifier character with the corresponding UTF8 chess
    # character
    UTF8_MAP = {
      'P' => '♙',
      'R' => '♖',
      'N' => '♘',
      'B' => '♗',
      'Q' => '♕',
      'K' => '♔',
      'p' => '♟',
      'r' => '♜',
      'n' => '♞',
      'b' => '♝',
      'q' => '♛',
      'k' => '♚'
    }.freeze

    # Replace the piece identifier characters with UTF8 chess characters.
    # @return [String]
    def to_utf8
      self.gsub(/[PRNBQKprnbqk]/, UTF8_MAP)
    end
  end
  # rubocop:enable Style/AsciiComments
end

String.prepend(Chess::UTF8Notation)
