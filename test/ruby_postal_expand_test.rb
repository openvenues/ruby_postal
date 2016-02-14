#!/bin/env ruby
# encoding: utf-8
require 'test_helper'
require 'set'

require 'ruby_postal/expand'

class RubyPostalExpandTest < Minitest::Test
    def self.is_contained_in_expansions?(address, output, options={})
        expansions = Postal::Expand.expand_address(address, options)
        expansions = Set.new expansions
        expansions.member?(output)
    end


    def self.has_expansion_in_common?(str1, str2, options={})
        expansions1 = Postal::Expand.expand_address(str1, options=options)
        expansions2 = Postal::Expand.expand_address(str2, options=options)

        s1 = Set.new expansions1
        s2 = Set.new expansions2
        s1.intersection(s2)
    end

    def test_expansions
        assert RubyPostalExpandTest.is_contained_in_expansions?("781 Franklin Ave Crown Hts Brooklyn NY", "781 franklin avenue crown heights brooklyn new york")
        assert RubyPostalExpandTest.is_contained_in_expansions?("Friedrichstraße 128, Berlin, Germany", "friedrich strasse 128 berlin germany", options={"languages" => ["de"]})
        assert RubyPostalExpandTest.has_expansion_in_common?("30 West Twenty-sixth Street Floor Number 7'", "Thirty W 26th St Fl #7", options={:languages => ["de"]})
    end
end
