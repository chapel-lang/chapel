use IO;
use CTypes;

require "c_utils.h", "c_utils.c";
extern proc point_to_char_array() : c_ptr(uint(8));
extern proc point_to_short_array() : c_ptr(uint(16));

var w = open("./cvoidptrdata.bin", iomode.cw).writer();

var p8 = point_to_char_array();
w.writeBinary(p8, 10);

var p16 = point_to_short_array();
w.writeBinary(p16, 20);
