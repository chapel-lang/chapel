// divfloor.chpl
// 
// Test the divfloor function.
// (Does not test the param version.)
//

var ui8b: uint(8) = 32;
var ui16b: uint(16) = 42441;
var ui32b: uint(32) = 3997024116;
var ui64b: uint(64) = 17914497524592412092;

var i8a: int(8) = -44;
var i8c: int(8) = 5;
var i16a: int(16) = -3322;
var i16c: int(16) = 5120;
var i32a: int(32) = -472289673;
var i32c: int(32) = 666765169;
var i64a: int(64) = -1996069246905712482;
var i64c: int(64) = 2625360699462582167;


writeln("ui8b = ", ui8b, " ui8b = ", ui8b, " divfloor(ui8b,ui8b) = ", divfloor(ui8b,ui8b));
writeln("ui8b = ", ui8b, " ui16b = ", ui16b, " divfloor(ui8b,ui16b) = ", divfloor(ui8b,ui16b));
writeln("ui8b = ", ui8b, " ui32b = ", ui32b, " divfloor(ui8b,ui32b) = ", divfloor(ui8b,ui32b));
//writeln("ui8b = ", ui8b, " ui64b = ", ui64b, " divfloor(ui8b,ui64b) = ", divfloor(ui8b,ui64b));
writeln("ui8b = ", ui8b, " i8a = ", i8a, " divfloor(ui8b,i8a) = ", divfloor(ui8b,i8a));
writeln("ui8b = ", ui8b, " i8c = ", i8c, " divfloor(ui8b,i8c) = ", divfloor(ui8b,i8c));
writeln("ui8b = ", ui8b, " i16a = ", i16a, " divfloor(ui8b,i16a) = ", divfloor(ui8b,i16a));
writeln("ui8b = ", ui8b, " i16c = ", i16c, " divfloor(ui8b,i16c) = ", divfloor(ui8b,i16c));
writeln("ui8b = ", ui8b, " i32a = ", i32a, " divfloor(ui8b,i32a) = ", divfloor(ui8b,i32a));
writeln("ui8b = ", ui8b, " i32c = ", i32c, " divfloor(ui8b,i32c) = ", divfloor(ui8b,i32c));
writeln("ui8b = ", ui8b, " i64a = ", i64a, " divfloor(ui8b,i64a) = ", divfloor(ui8b,i64a));
writeln("ui8b = ", ui8b, " i64c = ", i64c, " divfloor(ui8b,i64c) = ", divfloor(ui8b,i64c));

writeln("ui16b = ", ui16b, " ui8b = ", ui8b, " divfloor(ui16b,ui8b) = ", divfloor(ui16b,ui8b));
writeln("ui16b = ", ui16b, " ui16b = ", ui16b, " divfloor(ui16b,ui16b) = ", divfloor(ui16b,ui16b));
writeln("ui16b = ", ui16b, " ui32b = ", ui32b, " divfloor(ui16b,ui32b) = ", divfloor(ui16b,ui32b));
//writeln("ui16b = ", ui16b, " ui64b = ", ui64b, " divfloor(ui16b,ui64b) = ", divfloor(ui16b,ui64b));
writeln("ui16b = ", ui16b, " i8a = ", i8a, " divfloor(ui16b,i8a) = ", divfloor(ui16b,i8a));
writeln("ui16b = ", ui16b, " i8c = ", i8c, " divfloor(ui16b,i8c) = ", divfloor(ui16b,i8c));
writeln("ui16b = ", ui16b, " i16a = ", i16a, " divfloor(ui16b,i16a) = ", divfloor(ui16b,i16a));
writeln("ui16b = ", ui16b, " i16c = ", i16c, " divfloor(ui16b,i16c) = ", divfloor(ui16b,i16c));
writeln("ui16b = ", ui16b, " i32a = ", i32a, " divfloor(ui16b,i32a) = ", divfloor(ui16b,i32a));
writeln("ui16b = ", ui16b, " i32c = ", i32c, " divfloor(ui16b,i32c) = ", divfloor(ui16b,i32c));
writeln("ui16b = ", ui16b, " i64a = ", i64a, " divfloor(ui16b,i64a) = ", divfloor(ui16b,i64a));
writeln("ui16b = ", ui16b, " i64c = ", i64c, " divfloor(ui16b,i64c) = ", divfloor(ui16b,i64c));

writeln("ui32b = ", ui32b, " ui8b = ", ui8b, " divfloor(ui32b,ui8b) = ", divfloor(ui32b,ui8b));
writeln("ui32b = ", ui32b, " ui16b = ", ui16b, " divfloor(ui32b,ui16b) = ", divfloor(ui32b,ui16b));
writeln("ui32b = ", ui32b, " ui32b = ", ui32b, " divfloor(ui32b,ui32b) = ", divfloor(ui32b,ui32b));
//writeln("ui32b = ", ui32b, " ui64b = ", ui64b, " divfloor(ui32b,ui64b) = ", divfloor(ui32b,ui64b));
writeln("ui32b = ", ui32b, " i8a = ", i8a, " divfloor(ui32b,i8a) = ", divfloor(ui32b,i8a));
writeln("ui32b = ", ui32b, " i8c = ", i8c, " divfloor(ui32b,i8c) = ", divfloor(ui32b,i8c));
writeln("ui32b = ", ui32b, " i16a = ", i16a, " divfloor(ui32b,i16a) = ", divfloor(ui32b,i16a));
writeln("ui32b = ", ui32b, " i16c = ", i16c, " divfloor(ui32b,i16c) = ", divfloor(ui32b,i16c));
writeln("ui32b = ", ui32b, " i32a = ", i32a, " divfloor(ui32b,i32a) = ", divfloor(ui32b,i32a));
writeln("ui32b = ", ui32b, " i32c = ", i32c, " divfloor(ui32b,i32c) = ", divfloor(ui32b,i32c));
writeln("ui32b = ", ui32b, " i64a = ", i64a, " divfloor(ui32b,i64a) = ", divfloor(ui32b,i64a));
writeln("ui32b = ", ui32b, " i64c = ", i64c, " divfloor(ui32b,i64c) = ", divfloor(ui32b,i64c));

//writeln("ui64b = ", ui64b, " ui8b = ", ui8b, " divfloor(ui64b,ui8b) = ", divfloor(ui64b,ui8b));
//writeln("ui64b = ", ui64b, " ui16b = ", ui16b, " divfloor(ui64b,ui16b) = ", divfloor(ui64b,ui16b));
//writeln("ui64b = ", ui64b, " ui32b = ", ui32b, " divfloor(ui64b,ui32b) = ", divfloor(ui64b,ui32b));
//writeln("ui64b = ", ui64b, " ui64b = ", ui64b, " divfloor(ui64b,ui64b) = ", divfloor(ui64b,ui64b));
//writeln("ui64b = ", ui64b, " i8a = ", i8a, " divfloor(ui64b,i8a) = ", divfloor(ui64b,i8a));
//writeln("ui64b = ", ui64b, " i8c = ", i8c, " divfloor(ui64b,i8c) = ", divfloor(ui64b,i8c));
//writeln("ui64b = ", ui64b, " i16a = ", i16a, " divfloor(ui64b,i16a) = ", divfloor(ui64b,i16a));
//writeln("ui64b = ", ui64b, " i16c = ", i16c, " divfloor(ui64b,i16c) = ", divfloor(ui64b,i16c));
//writeln("ui64b = ", ui64b, " i32a = ", i32a, " divfloor(ui64b,i32a) = ", divfloor(ui64b,i32a));
//writeln("ui64b = ", ui64b, " i32c = ", i32c, " divfloor(ui64b,i32c) = ", divfloor(ui64b,i32c));
//writeln("ui64b = ", ui64b, " i64a = ", i64a, " divfloor(ui64b,i64a) = ", divfloor(ui64b,i64a));
//writeln("ui64b = ", ui64b, " i64c = ", i64c, " divfloor(ui64b,i64c) = ", divfloor(ui64b,i64c));

writeln("i8a = ", i8a, " ui8b = ", ui8b, " divfloor(i8a,ui8b) = ", divfloor(i8a,ui8b));
writeln("i8a = ", i8a, " ui16b = ", ui16b, " divfloor(i8a,ui16b) = ", divfloor(i8a,ui16b));
writeln("i8a = ", i8a, " ui32b = ", ui32b, " divfloor(i8a,ui32b) = ", divfloor(i8a,ui32b));
//writeln("i8a = ", i8a, " ui64b = ", ui64b, " divfloor(i8a,ui64b) = ", divfloor(i8a,ui64b));
writeln("i8a = ", i8a, " i8a = ", i8a, " divfloor(i8a,i8a) = ", divfloor(i8a,i8a));
writeln("i8a = ", i8a, " i8c = ", i8c, " divfloor(i8a,i8c) = ", divfloor(i8a,i8c));
writeln("i8a = ", i8a, " i16a = ", i16a, " divfloor(i8a,i16a) = ", divfloor(i8a,i16a));
writeln("i8a = ", i8a, " i16c = ", i16c, " divfloor(i8a,i16c) = ", divfloor(i8a,i16c));
writeln("i8a = ", i8a, " i32a = ", i32a, " divfloor(i8a,i32a) = ", divfloor(i8a,i32a));
writeln("i8a = ", i8a, " i32c = ", i32c, " divfloor(i8a,i32c) = ", divfloor(i8a,i32c));
writeln("i8a = ", i8a, " i64a = ", i64a, " divfloor(i8a,i64a) = ", divfloor(i8a,i64a));
writeln("i8a = ", i8a, " i64c = ", i64c, " divfloor(i8a,i64c) = ", divfloor(i8a,i64c));

writeln("i8c = ", i8c, " ui8b = ", ui8b, " divfloor(i8c,ui8b) = ", divfloor(i8c,ui8b));
writeln("i8c = ", i8c, " ui16b = ", ui16b, " divfloor(i8c,ui16b) = ", divfloor(i8c,ui16b));
writeln("i8c = ", i8c, " ui32b = ", ui32b, " divfloor(i8c,ui32b) = ", divfloor(i8c,ui32b));
//writeln("i8c = ", i8c, " ui64b = ", ui64b, " divfloor(i8c,ui64b) = ", divfloor(i8c,ui64b));
writeln("i8c = ", i8c, " i8a = ", i8a, " divfloor(i8c,i8a) = ", divfloor(i8c,i8a));
writeln("i8c = ", i8c, " i8c = ", i8c, " divfloor(i8c,i8c) = ", divfloor(i8c,i8c));
writeln("i8c = ", i8c, " i16a = ", i16a, " divfloor(i8c,i16a) = ", divfloor(i8c,i16a));
writeln("i8c = ", i8c, " i16c = ", i16c, " divfloor(i8c,i16c) = ", divfloor(i8c,i16c));
writeln("i8c = ", i8c, " i32a = ", i32a, " divfloor(i8c,i32a) = ", divfloor(i8c,i32a));
writeln("i8c = ", i8c, " i32c = ", i32c, " divfloor(i8c,i32c) = ", divfloor(i8c,i32c));
writeln("i8c = ", i8c, " i64a = ", i64a, " divfloor(i8c,i64a) = ", divfloor(i8c,i64a));
writeln("i8c = ", i8c, " i64c = ", i64c, " divfloor(i8c,i64c) = ", divfloor(i8c,i64c));

writeln("i16a = ", i16a, " ui8b = ", ui8b, " divfloor(i16a,ui8b) = ", divfloor(i16a,ui8b));
writeln("i16a = ", i16a, " ui16b = ", ui16b, " divfloor(i16a,ui16b) = ", divfloor(i16a,ui16b));
writeln("i16a = ", i16a, " ui32b = ", ui32b, " divfloor(i16a,ui32b) = ", divfloor(i16a,ui32b));
//writeln("i16a = ", i16a, " ui64b = ", ui64b, " divfloor(i16a,ui64b) = ", divfloor(i16a,ui64b));
writeln("i16a = ", i16a, " i8a = ", i8a, " divfloor(i16a,i8a) = ", divfloor(i16a,i8a));
writeln("i16a = ", i16a, " i8c = ", i8c, " divfloor(i16a,i8c) = ", divfloor(i16a,i8c));
writeln("i16a = ", i16a, " i16a = ", i16a, " divfloor(i16a,i16a) = ", divfloor(i16a,i16a));
writeln("i16a = ", i16a, " i16c = ", i16c, " divfloor(i16a,i16c) = ", divfloor(i16a,i16c));
writeln("i16a = ", i16a, " i32a = ", i32a, " divfloor(i16a,i32a) = ", divfloor(i16a,i32a));
writeln("i16a = ", i16a, " i32c = ", i32c, " divfloor(i16a,i32c) = ", divfloor(i16a,i32c));
writeln("i16a = ", i16a, " i64a = ", i64a, " divfloor(i16a,i64a) = ", divfloor(i16a,i64a));
writeln("i16a = ", i16a, " i64c = ", i64c, " divfloor(i16a,i64c) = ", divfloor(i16a,i64c));

writeln("i16c = ", i16c, " ui8b = ", ui8b, " divfloor(i16c,ui8b) = ", divfloor(i16c,ui8b));
writeln("i16c = ", i16c, " ui16b = ", ui16b, " divfloor(i16c,ui16b) = ", divfloor(i16c,ui16b));
writeln("i16c = ", i16c, " ui32b = ", ui32b, " divfloor(i16c,ui32b) = ", divfloor(i16c,ui32b));
//writeln("i16c = ", i16c, " ui64b = ", ui64b, " divfloor(i16c,ui64b) = ", divfloor(i16c,ui64b));
writeln("i16c = ", i16c, " i8a = ", i8a, " divfloor(i16c,i8a) = ", divfloor(i16c,i8a));
writeln("i16c = ", i16c, " i8c = ", i8c, " divfloor(i16c,i8c) = ", divfloor(i16c,i8c));
writeln("i16c = ", i16c, " i16a = ", i16a, " divfloor(i16c,i16a) = ", divfloor(i16c,i16a));
writeln("i16c = ", i16c, " i16c = ", i16c, " divfloor(i16c,i16c) = ", divfloor(i16c,i16c));
writeln("i16c = ", i16c, " i32a = ", i32a, " divfloor(i16c,i32a) = ", divfloor(i16c,i32a));
writeln("i16c = ", i16c, " i32c = ", i32c, " divfloor(i16c,i32c) = ", divfloor(i16c,i32c));
writeln("i16c = ", i16c, " i64a = ", i64a, " divfloor(i16c,i64a) = ", divfloor(i16c,i64a));
writeln("i16c = ", i16c, " i64c = ", i64c, " divfloor(i16c,i64c) = ", divfloor(i16c,i64c));

writeln("i32a = ", i32a, " ui8b = ", ui8b, " divfloor(i32a,ui8b) = ", divfloor(i32a,ui8b));
writeln("i32a = ", i32a, " ui16b = ", ui16b, " divfloor(i32a,ui16b) = ", divfloor(i32a,ui16b));
writeln("i32a = ", i32a, " ui32b = ", ui32b, " divfloor(i32a,ui32b) = ", divfloor(i32a,ui32b));
//writeln("i32a = ", i32a, " ui64b = ", ui64b, " divfloor(i32a,ui64b) = ", divfloor(i32a,ui64b));
writeln("i32a = ", i32a, " i8a = ", i8a, " divfloor(i32a,i8a) = ", divfloor(i32a,i8a));
writeln("i32a = ", i32a, " i8c = ", i8c, " divfloor(i32a,i8c) = ", divfloor(i32a,i8c));
writeln("i32a = ", i32a, " i16a = ", i16a, " divfloor(i32a,i16a) = ", divfloor(i32a,i16a));
writeln("i32a = ", i32a, " i16c = ", i16c, " divfloor(i32a,i16c) = ", divfloor(i32a,i16c));
writeln("i32a = ", i32a, " i32a = ", i32a, " divfloor(i32a,i32a) = ", divfloor(i32a,i32a));
writeln("i32a = ", i32a, " i32c = ", i32c, " divfloor(i32a,i32c) = ", divfloor(i32a,i32c));
writeln("i32a = ", i32a, " i64a = ", i64a, " divfloor(i32a,i64a) = ", divfloor(i32a,i64a));
writeln("i32a = ", i32a, " i64c = ", i64c, " divfloor(i32a,i64c) = ", divfloor(i32a,i64c));

writeln("i32c = ", i32c, " ui8b = ", ui8b, " divfloor(i32c,ui8b) = ", divfloor(i32c,ui8b));
writeln("i32c = ", i32c, " ui16b = ", ui16b, " divfloor(i32c,ui16b) = ", divfloor(i32c,ui16b));
writeln("i32c = ", i32c, " ui32b = ", ui32b, " divfloor(i32c,ui32b) = ", divfloor(i32c,ui32b));
//writeln("i32c = ", i32c, " ui64b = ", ui64b, " divfloor(i32c,ui64b) = ", divfloor(i32c,ui64b));
writeln("i32c = ", i32c, " i8a = ", i8a, " divfloor(i32c,i8a) = ", divfloor(i32c,i8a));
writeln("i32c = ", i32c, " i8c = ", i8c, " divfloor(i32c,i8c) = ", divfloor(i32c,i8c));
writeln("i32c = ", i32c, " i16a = ", i16a, " divfloor(i32c,i16a) = ", divfloor(i32c,i16a));
writeln("i32c = ", i32c, " i16c = ", i16c, " divfloor(i32c,i16c) = ", divfloor(i32c,i16c));
writeln("i32c = ", i32c, " i32a = ", i32a, " divfloor(i32c,i32a) = ", divfloor(i32c,i32a));
writeln("i32c = ", i32c, " i32c = ", i32c, " divfloor(i32c,i32c) = ", divfloor(i32c,i32c));
writeln("i32c = ", i32c, " i64a = ", i64a, " divfloor(i32c,i64a) = ", divfloor(i32c,i64a));
writeln("i32c = ", i32c, " i64c = ", i64c, " divfloor(i32c,i64c) = ", divfloor(i32c,i64c));

writeln("i64a = ", i64a, " ui8b = ", ui8b, " divfloor(i64a,ui8b) = ", divfloor(i64a,ui8b));
writeln("i64a = ", i64a, " ui16b = ", ui16b, " divfloor(i64a,ui16b) = ", divfloor(i64a,ui16b));
writeln("i64a = ", i64a, " ui32b = ", ui32b, " divfloor(i64a,ui32b) = ", divfloor(i64a,ui32b));
//writeln("i64a = ", i64a, " ui64b = ", ui64b, " divfloor(i64a,ui64b) = ", divfloor(i64a,ui64b));
writeln("i64a = ", i64a, " i8a = ", i8a, " divfloor(i64a,i8a) = ", divfloor(i64a,i8a));
writeln("i64a = ", i64a, " i8c = ", i8c, " divfloor(i64a,i8c) = ", divfloor(i64a,i8c));
writeln("i64a = ", i64a, " i16a = ", i16a, " divfloor(i64a,i16a) = ", divfloor(i64a,i16a));
writeln("i64a = ", i64a, " i16c = ", i16c, " divfloor(i64a,i16c) = ", divfloor(i64a,i16c));
writeln("i64a = ", i64a, " i32a = ", i32a, " divfloor(i64a,i32a) = ", divfloor(i64a,i32a));
writeln("i64a = ", i64a, " i32c = ", i32c, " divfloor(i64a,i32c) = ", divfloor(i64a,i32c));
writeln("i64a = ", i64a, " i64a = ", i64a, " divfloor(i64a,i64a) = ", divfloor(i64a,i64a));
writeln("i64a = ", i64a, " i64c = ", i64c, " divfloor(i64a,i64c) = ", divfloor(i64a,i64c));

writeln("i64c = ", i64c, " ui8b = ", ui8b, " divfloor(i64c,ui8b) = ", divfloor(i64c,ui8b));
writeln("i64c = ", i64c, " ui16b = ", ui16b, " divfloor(i64c,ui16b) = ", divfloor(i64c,ui16b));
writeln("i64c = ", i64c, " ui32b = ", ui32b, " divfloor(i64c,ui32b) = ", divfloor(i64c,ui32b));
//writeln("i64c = ", i64c, " ui64b = ", ui64b, " divfloor(i64c,ui64b) = ", divfloor(i64c,ui64b));
writeln("i64c = ", i64c, " i8a = ", i8a, " divfloor(i64c,i8a) = ", divfloor(i64c,i8a));
writeln("i64c = ", i64c, " i8c = ", i8c, " divfloor(i64c,i8c) = ", divfloor(i64c,i8c));
writeln("i64c = ", i64c, " i16a = ", i16a, " divfloor(i64c,i16a) = ", divfloor(i64c,i16a));
writeln("i64c = ", i64c, " i16c = ", i16c, " divfloor(i64c,i16c) = ", divfloor(i64c,i16c));
writeln("i64c = ", i64c, " i32a = ", i32a, " divfloor(i64c,i32a) = ", divfloor(i64c,i32a));
writeln("i64c = ", i64c, " i32c = ", i32c, " divfloor(i64c,i32c) = ", divfloor(i64c,i32c));
writeln("i64c = ", i64c, " i64a = ", i64a, " divfloor(i64c,i64a) = ", divfloor(i64c,i64a));
writeln("i64c = ", i64c, " i64c = ", i64c, " divfloor(i64c,i64c) = ", divfloor(i64c,i64c));
