require 'ruby_postal/cpostal/expand'

module Postal
    class Expand
        def self.expand_address(address, options={})
            CExpand.expand_address (address or ""), options
        end
    end
end
