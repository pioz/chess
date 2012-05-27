require 'bundler/gem_tasks'

require 'rdoc/task'
Rake::RDocTask.new do |rd|
  rd.title = 'Chess'
  rd.main = 'README.rdoc'
  rd.rdoc_dir = 'doc'
  rd.rdoc_files.include('README.rdoc', 'lib/**/*.rb', 'ext/*.c')
end

require 'rake/testtask'
task :default => :test
Rake::TestTask.new do |test|
  test.verbose = true
  test.libs << 'test'
  test.libs << 'lib'
  test.test_files = FileList['test/test*.rb']
end
%w(collection to_check).each do |test_name|
  Rake::TestTask.new("test:#{test_name}") do |test|
    test.verbose = true
    test.libs << 'test'
    test.libs << 'lib'
    test.test_files = FileList["test/test_pgn_#{test_name}.rb"]
  end
end