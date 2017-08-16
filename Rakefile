require 'bundler/gem_tasks'

require 'yard'
YARD::Rake::YardocTask.new do |t|
 t.files = ['lib/**/*.rb', 'ext/*.c']
 t.options << '-rREADME.md'
 t.options << '--title=Chess'
 t.options << '-mmarkdown'
end

require 'rake/testtask'
task default: :test
Rake::TestTask.new do |test|
  test.libs << 'test'
  test.warning = true
  test.test_files = FileList["test/test_#{ENV['T'] || '*'}.rb"]
end
