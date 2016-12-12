require 'ruby_postal/cpostal/expand'

module Postal
    class Expand
        def self.expand_address(address, options={})
            if not address
                return []
            end
            CExpand.expand_address address, options
        end
    end
end
