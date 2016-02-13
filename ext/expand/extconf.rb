require 'mkmf'

$LIBS << " -lpostal"

unless find_header('libpostal/libpostal.h')
  abort "libpostal is missing.  please install"
end


create_makefile('cpostal/expand')