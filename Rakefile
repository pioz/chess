require 'bundler/gem_tasks'

require 'rdoc/task'
Rake::RDocTask.new do |rd|
  rd.rdoc_files.include('README.rdoc', 'LICENSE', 'lib/**/*.rb', 'ext/*.c')
  rd.main = 'README.rdoc'
  rd.title = 'Chess'
  rd.template = 'darkfish'
  rd.rdoc_dir = 'doc'
end

require 'rake/testtask'
task default: :test
Rake::TestTask.new do |test|
  test.libs << 'test'
  test.warning = true
  test.test_files = FileList["test/test_#{ENV['T'] || '*'}.rb"]
end
