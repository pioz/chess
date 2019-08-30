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
  '/usr/include'
].freeze

LIB_DIRS = [
  # Check the ruby install locations
  RbConfig::CONFIG['libdir'],
  # First search /opt/local for macports
  '/opt/local/lib',
  # Then search /usr/local for people that installed from source
  '/usr/local/lib',
  # Finally fall back to /usr
  '/usr/lib'
].freeze

# dir_config('chess', INCLUDE_DIRS, LIB_DIRS)
$CFLAGS << ' -std=c99'

create_makefile('chess/chess')
