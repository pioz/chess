$LOAD_PATH.push File.expand_path('lib', __dir__)
require 'chess/version'

Gem::Specification.new do |s|
  s.name        = 'chess'
  s.version     = Chess::VERSION
  s.authors     = ['Enrico Pilotto']
  s.email       = ['epilotto@gmx.com']
  s.homepage    = 'https://github.com/pioz/chess'
  s.summary     = 'A fast chess library to play chess with Ruby.'
  s.description = 'A fast chess library that use bitboards to play chess with Ruby.'
  s.license     = 'LGPLv3'

  s.rubyforge_project = 'chess'

  s.files         = `git ls-files`.split("\n")
  s.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  s.executables   = `git ls-files -- bin/*`.split("\n").map { |f| File.basename(f) }
  s.extensions    = ['ext/extconf.rb']
  s.require_paths = ['lib']

  s.required_ruby_version = '>= 2.0'
  s.add_development_dependency 'bundler', '~> 2'
  s.add_development_dependency 'minitest', '~> 5'
  s.add_development_dependency 'rake', '~> 12'
  s.add_development_dependency 'rubocop', '~> 0.71'
  s.add_development_dependency 'rubocop-performance', '~> 1'
  s.add_development_dependency 'yard', '~> 0.9'
end
