#!/bin/env ruby
# encoding: utf-8

require 'test_helper'
require 'ruby_postal/parser'

class RubyPostalParserTest < Minitest::Test
    def self.has_all_components(address, components)
        expected = components.length
        got = 0

        parsed = Postal::Parser.parse_address(address)
        parsed.each { |comp| got += components[comp[:label]] == comp[:value] ? 1 : 0}

        got == expected
    end

    def test_parser
        assert RubyPostalParserTest.has_all_components('781 Franklin Ave Crown Heights Brooklyn NYC NY 11216 USA', {
                :house_number => '781',
                :road => 'franklin ave',
                :suburb => 'crown heights',
                :city_district => 'brooklyn',
                :city => 'nyc',
                :state => 'ny',
                :postcode => '11216',
                :country => 'usa'

            })
    end
end
