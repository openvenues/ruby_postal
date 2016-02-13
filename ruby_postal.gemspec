# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'ruby_postal/version'

Gem::Specification.new do |spec|
  spec.name          = "ruby_postal"
  spec.version       = Postal::VERSION
  spec.authors       = ["Al Barrentine"]
  spec.email         = ["pelias@mapzen.com"]

  spec.summary       = "Ruby bindings for libpostal (fast address parsing/normalization)"
  spec.homepage      = "https://github.com/openvenues/ruby_postal"
  spec.license       = "MIT"

  # Prevent pushing this gem to RubyGems.org by setting 'allowed_push_host', or
  # delete this section to allow pushing this gem to any host.
  if spec.respond_to?(:metadata)
    spec.metadata['allowed_push_host'] = "TODO: Set to 'http://mygemserver.com'"
  else
    raise "RubyGems 2.0 or newer is required to protect against public gem pushes."
  end

  spec.files = Dir['lib/**/*.rb'] + Dir['ext/**/*.c'] + Dir['ext/**/extconf.rb']
  spec.require_paths = [ 'lib', 'ext' ]
  spec.extensions = Dir['ext/**/extconf.rb']

  spec.add_development_dependency "bundler"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "minitest"
end
