require 'ruby_postal/cpostal/parser'

module Postal
    class Parser
        def self.parse_address(address, options={})
            if not address
                return []
            end
            parse_result = CParser.parse_address address, options
            if address.respond_to?(:encode)
                address = address.encode("UTF-8")
            end
            parse_result.map{|s, c| {:label => c, :value => s.force_encoding("UTF-8")}}
        end
    end
end
