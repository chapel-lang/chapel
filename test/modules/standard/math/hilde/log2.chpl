// log2.chpl
// 
// Test the log2 function.
//

var ui8a: uint(8) = 0;
var ui8b: uint(8) = 32;
var ui16a: uint(16) = 0;
var ui16b: uint(16) = 42441;
var ui32a: uint(32) = 0;
var ui32b: uint(32) = 3997024116;
var ui64a: uint(64) = 0;
var ui64b: uint(64) = 17914497524592412092;

var i8a: int(8) = -44;
var i8b: int(8) = 0;
var i8c: int(8) = 5;
var i16a: int(16) = -3322;
var i16b: int(16) = 0;
var i16c: int(16) = 5120;
var i32a: int(32) = -472289673;
var i32b: int(32) = 0;
var i32c: int(32) = 666765169;
var i64a: int(64) = -1996069246905712482;
var i64b: int(64) = 0;
var i64c: int(64) = 2625360699462582167;

var re32a: real(32) = -0.653338667:real(32);
var re32b: real(32) = -0.0:real(32);
var re32c: real(32) = 0.0:real(32);
var re32d: real(32) = 0.814420537:real(32);

var re64a: real(64) = -0.988837303;
var re64b: real(64) = -0.0;
var re64c: real(64) = 0.0;
var re64d: real(64) = 0.153876293;


//writeln("ui8a = ", ui8a, " log2(ui8a) = ", log2(ui8a));
writeln("ui8b = ", ui8b, " log2(ui8b) = ", log2(ui8b));
//writeln("ui16a = ", ui16a, " log2(ui16a) = ", log2(ui16a));
writeln("ui16b = ", ui16b, " log2(ui16b) = ", log2(ui16b));
//writeln("ui32a = ", ui32a, " log2(ui32a) = ", log2(ui32a));
writeln("ui32b = ", ui32b, " log2(ui32b) = ", log2(ui32b));
//writeln("ui64a = ", ui64a, " log2(ui64a) = ", log2(ui64a));
writeln("ui64b = ", ui64b, " log2(ui64b) = ", log2(ui64b));

//writeln("i8a = ", i8a, " log2(i8a) = ", log2(i8a));
//writeln("i8b = ", i8b, " log2(i8b) = ", log2(i8b));
writeln("i8c = ", i8c, " log2(i8c) = ", log2(i8c));
//writeln("i16a = ", i16a, " log2(i16a) = ", log2(i16a));
//writeln("i16b = ", i16b, " log2(i16b) = ", log2(i16b));
writeln("i16c = ", i16c, " log2(i16c) = ", log2(i16c));
//writeln("i32a = ", i32a, " log2(i32a) = ", log2(i32a));
//writeln("i32b = ", i32b, " log2(i32b) = ", log2(i32b));
writeln("i32c = ", i32c, " log2(i32c) = ", log2(i32c));
//writeln("i64a = ", i64a, " log2(i64a) = ", log2(i64a));
//writeln("i64b = ", i64b, " log2(i64b) = ", log2(i64b));
writeln("i64c = ", i64c, " log2(i64c) = ", log2(i64c));

writeln("re32a = ", re32a, " log2(re32a) = ", log2(re32a));
writeln("re32b = ", re32b, " log2(re32b) = ", log2(re32b));
writeln("re32c = ", re32c, " log2(re32c) = ", log2(re32c));
writeln("re32d = ", re32d, " log2(re32d) = ", log2(re32d));

writeln("re64a = ", re64a, " log2(re64a) = ", log2(re64a));
writeln("re64b = ", re64b, " log2(re64b) = ", log2(re64b));
writeln("re64c = ", re64c, " log2(re64c) = ", log2(re64c));
writeln("re64d = ", re64d, " log2(re64d) = ", log2(re64d));

