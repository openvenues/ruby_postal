# ruby_postal

[![Build Status](https://travis-ci.org/openvenues/ruby_postal.svg?branch=master)](https://travis-ci.org/openvenues/ruby_postal)

These are the Ruby bindings to [libpostal](https://github.com/openvenues/libpostal) for
fast street address parsing and normalization.

## Usage

To expand address strings into normalized forms suitable for geocoder queries:

```ruby
require 'ruby_postal/expand'
Postal::Expand.expand_address('Quatre-vignt-douze Ave des Ave des Champs-Élysées')
```

To parse addresses into components:

```ruby
require 'ruby_postal/parser'
Postal::Parser.parse_address('781 Franklin Ave Crown Heights Brooklyn NY 11216 USA')
```

## Prerequisites

Before using the Ruby bindings, you must install the libpostal C library. Make sure you have the following prerequisites:

**On Linux (Ubuntu)**
```
sudo apt-get install libsnappy-dev autoconf automake libtool
```

**On Mac OSX**
```
sudo brew install snappy autoconf automake libtool
```

**Installing libpostal**

```
git clone https://github.com/openvenues/libpostal
cd libpostal
./bootstrap.sh
./configure --datadir=[...some dir with a few GB of space...]
make
sudo make install

# On Linux it's probably a good idea to run
sudo ldconfig
```

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'ruby_postal'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install ruby_postal

## Development

```
bundle install
bundle exec rake
```

If you installed libpostal to a non-standard location, use:

```
bundle exec rake --with-libpostal-dir=/libpostal/prefix
```

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Tests

```
rake test
```

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/openvenues/ruby_postal.

## License

The gem is available as open source under the terms of the [MIT License](http://opensource.org/licenses/MIT).

