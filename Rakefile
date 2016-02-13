require "bundler/gem_tasks"
require "rake/testtask"
require 'rake/extensiontask'

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList['test/**/*_test.rb']
end

Rake::ExtensionTask.new do |ext|
      ext.name = 'expand'
      ext.ext_dir = 'ext/expand'
      ext.lib_dir = 'lib/ruby_postal/cpostal'
end

Rake::ExtensionTask.new do |ext|
      ext.name = 'parser'
      ext.ext_dir = 'ext/parser'
      ext.lib_dir = 'lib/ruby_postal/cpostal'
end

task :default => :spec
