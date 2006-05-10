// Test the various integer sizes, 8-128 bits
var i8  :int(8) = 1<<7;
var i16 :int(16) = 1<<15;
var i32 :int(32) = 1<<31;
var i64 :int(64) = 1<<63;

writeln ("int(8) can hold ", i8);
writeln ("int(16) can hold ", i16);
writeln ("int(32) can hold ", i32);
writeln ("int(64) can hold ", i64);
