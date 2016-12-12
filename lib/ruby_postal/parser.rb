require 'ruby_postal/cpostal/parser'

module Postal
    class Parser
        def self.parse_address(address, options={})
            parse_result = CParser.parse_address address.to_s, options
            parse_result.map{|s, c| {:label => c, :value => s}}
        end
    end
end
