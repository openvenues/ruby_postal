require 'ruby_postal/cpostal/expand'

module Postal
    class Expand
        def self.expand_address(address, options={})
            if not address
                return []
            end
            if address.respond_to?(:encode)
                address = address.encode("UTF-8")
            end
            expand_result = CExpand.expand_address address, options
            expand_result.map{|s| s.force_encoding("UTF-8")}

        end
    end
end
