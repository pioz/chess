# Creating a gem that includes an extension that is built at install time
# https://guides.rubygems.org/gems-with-extensions/

require 'mkmf'

$CFLAGS += ' -std=c99'

create_makefile('chess/chess')
