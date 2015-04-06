// sgn.chpl
// 
// Test the sgn function.
//  (Does not test the param version.)
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


writeln("ui8a = ", ui8a, " sgn(ui8a) = ", sgn(ui8a));
writeln("ui8b = ", ui8b, " sgn(ui8b) = ", sgn(ui8b));
writeln("ui16a = ", ui16a, " sgn(ui16a) = ", sgn(ui16a));
writeln("ui16b = ", ui16b, " sgn(ui16b) = ", sgn(ui16b));
writeln("ui32a = ", ui32a, " sgn(ui32a) = ", sgn(ui32a));
writeln("ui32b = ", ui32b, " sgn(ui32b) = ", sgn(ui32b));
writeln("ui64a = ", ui64a, " sgn(ui64a) = ", sgn(ui64a));
writeln("ui64b = ", ui64b, " sgn(ui64b) = ", sgn(ui64b));

writeln("i8a = ", i8a, " sgn(i8a) = ", sgn(i8a));
writeln("i8b = ", i8b, " sgn(i8b) = ", sgn(i8b));
writeln("i8c = ", i8c, " sgn(i8c) = ", sgn(i8c));
writeln("i16a = ", i16a, " sgn(i16a) = ", sgn(i16a));
writeln("i16b = ", i16b, " sgn(i16b) = ", sgn(i16b));
writeln("i16c = ", i16c, " sgn(i16c) = ", sgn(i16c));
writeln("i32a = ", i32a, " sgn(i32a) = ", sgn(i32a));
writeln("i32b = ", i32b, " sgn(i32b) = ", sgn(i32b));
writeln("i32c = ", i32c, " sgn(i32c) = ", sgn(i32c));
writeln("i64a = ", i64a, " sgn(i64a) = ", sgn(i64a));
writeln("i64b = ", i64b, " sgn(i64b) = ", sgn(i64b));
writeln("i64c = ", i64c, " sgn(i64c) = ", sgn(i64c));

writeln("re32a = ", re32a, " sgn(re32a) = ", sgn(re32a));
writeln("re32b = ", re32b, " sgn(re32b) = ", sgn(re32b));
writeln("re32c = ", re32c, " sgn(re32c) = ", sgn(re32c));
writeln("re32d = ", re32d, " sgn(re32d) = ", sgn(re32d));

writeln("re64a = ", re64a, " sgn(re64a) = ", sgn(re64a));
writeln("re64b = ", re64b, " sgn(re64b) = ", sgn(re64b));
writeln("re64c = ", re64c, " sgn(re64c) = ", sgn(re64c));
writeln("re64d = ", re64d, " sgn(re64d) = ", sgn(re64d));

