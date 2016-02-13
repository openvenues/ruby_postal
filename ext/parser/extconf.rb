require 'mkmf'

$CFLAGS << " -I/usr/local/include"
$LIBS << " -L/usr/local/lib -lpostal"

unless find_header('libpostal/libpostal.h')
  abort "libpostal is missing.  please install"
end

create_makefile('cpostal/parser')