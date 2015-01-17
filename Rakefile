require 'bundler/gem_tasks'

require 'rdoc/task'
Rake::RDocTask.new do |rd|
  rd.title = 'Chess'
  rd.main = 'README.rdoc'
  rd.rdoc_dir = 'doc'
  rd.rdoc_files.include('README.rdoc', 'lib/**/*.rb', 'ext/*.c')
  #rd.options << '-f' << 'horo'
end

require 'rake/testtask'
task :default => :test
Rake::TestTask.new do |test|
  test.libs << 'test'
  test.test_files = FileList["test/test_#{ENV['T'] || '*'}.rb"]
end
