use IO;
use CTypes;

require "c_utils.h", "c_utils.c";
extern proc point_to_void_char_array() : c_void_ptr;
extern proc point_to_void_short_array() : c_void_ptr;

var w = open("./cptrdata.bin", iomode.cw).writer();

var pv8 = point_to_void_char_array();
w.writeBinary(pv8, 10);

var pv16 = point_to_void_short_array();
w.writeBinary(pv16, 20);
