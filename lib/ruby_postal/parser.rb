require 'cpostal/parser'

module Postal
    class Parser
        def self.parse_address(address, options={})
            parse_result = CParser.parse_address address, options
            parse_result.map{|s, c| {:component => s, :label => c}}
        end
    end
end