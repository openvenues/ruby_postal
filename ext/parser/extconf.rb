require 'mkmf'

$LIBS << " -lpostal"

create_makefile('cpostal/parser')