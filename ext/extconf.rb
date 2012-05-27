# :stopdoc:

ENV['RC_ARCHS'] = '' if RUBY_PLATFORM =~ /darwin/

require 'mkmf'

INCLUDE_DIRS = [
  # Check the ruby install locations
  RbConfig::CONFIG['includedir'],
  # First earch /opt/local for macports
  '/opt/local/include',
  # Then search /usr/local for people that installed from source
  '/usr/local/include',
  # Finally fall back to /usr
  '/usr/include',
]

LIB_DIRS = [
  # Check the ruby install locations
  RbConfig::CONFIG['libdir'],
  # First search /opt/local for macports
  '/opt/local/lib',
  # Then search /usr/local for people that installed from source
  '/usr/local/lib',
  # Finally fall back to /usr
  '/usr/lib',
]

#dir_config('chess', INCLUDE_DIRS, LIB_DIRS)

create_makefile('chess/chess')

# :startdoc: