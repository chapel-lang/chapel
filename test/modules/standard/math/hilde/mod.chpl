// mod.chpl
// 
// Test the mod function.
// (Does no test the param version.)
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


writeln("ui8b = ", ui8b, " ui8b = ", ui8b, " mod(ui8b,ui8b) = ", mod(ui8b,ui8b));
writeln("ui8b = ", ui8b, " ui16b = ", ui16b, " mod(ui8b,ui16b) = ", mod(ui8b,ui16b));
writeln("ui8b = ", ui8b, " ui32b = ", ui32b, " mod(ui8b,ui32b) = ", mod(ui8b,ui32b));
//writeln("ui8b = ", ui8b, " ui64b = ", ui64b, " mod(ui8b,ui64b) = ", mod(ui8b,ui64b));
writeln("ui8b = ", ui8b, " i8a = ", i8a, " mod(ui8b,i8a) = ", mod(ui8b,i8a));
writeln("ui8b = ", ui8b, " i8c = ", i8c, " mod(ui8b,i8c) = ", mod(ui8b,i8c));
writeln("ui8b = ", ui8b, " i16a = ", i16a, " mod(ui8b,i16a) = ", mod(ui8b,i16a));
writeln("ui8b = ", ui8b, " i16c = ", i16c, " mod(ui8b,i16c) = ", mod(ui8b,i16c));
writeln("ui8b = ", ui8b, " i32a = ", i32a, " mod(ui8b,i32a) = ", mod(ui8b,i32a));
writeln("ui8b = ", ui8b, " i32c = ", i32c, " mod(ui8b,i32c) = ", mod(ui8b,i32c));
writeln("ui8b = ", ui8b, " i64a = ", i64a, " mod(ui8b,i64a) = ", mod(ui8b,i64a));
writeln("ui8b = ", ui8b, " i64c = ", i64c, " mod(ui8b,i64c) = ", mod(ui8b,i64c));

writeln("ui16b = ", ui16b, " ui8b = ", ui8b, " mod(ui16b,ui8b) = ", mod(ui16b,ui8b));
writeln("ui16b = ", ui16b, " ui16b = ", ui16b, " mod(ui16b,ui16b) = ", mod(ui16b,ui16b));
writeln("ui16b = ", ui16b, " ui32b = ", ui32b, " mod(ui16b,ui32b) = ", mod(ui16b,ui32b));
//writeln("ui16b = ", ui16b, " ui64b = ", ui64b, " mod(ui16b,ui64b) = ", mod(ui16b,ui64b));
writeln("ui16b = ", ui16b, " i8a = ", i8a, " mod(ui16b,i8a) = ", mod(ui16b,i8a));
writeln("ui16b = ", ui16b, " i8c = ", i8c, " mod(ui16b,i8c) = ", mod(ui16b,i8c));
writeln("ui16b = ", ui16b, " i16a = ", i16a, " mod(ui16b,i16a) = ", mod(ui16b,i16a));
writeln("ui16b = ", ui16b, " i16c = ", i16c, " mod(ui16b,i16c) = ", mod(ui16b,i16c));
writeln("ui16b = ", ui16b, " i32a = ", i32a, " mod(ui16b,i32a) = ", mod(ui16b,i32a));
writeln("ui16b = ", ui16b, " i32c = ", i32c, " mod(ui16b,i32c) = ", mod(ui16b,i32c));
writeln("ui16b = ", ui16b, " i64a = ", i64a, " mod(ui16b,i64a) = ", mod(ui16b,i64a));
writeln("ui16b = ", ui16b, " i64c = ", i64c, " mod(ui16b,i64c) = ", mod(ui16b,i64c));

writeln("ui32b = ", ui32b, " ui8b = ", ui8b, " mod(ui32b,ui8b) = ", mod(ui32b,ui8b));
writeln("ui32b = ", ui32b, " ui16b = ", ui16b, " mod(ui32b,ui16b) = ", mod(ui32b,ui16b));
writeln("ui32b = ", ui32b, " ui32b = ", ui32b, " mod(ui32b,ui32b) = ", mod(ui32b,ui32b));
//writeln("ui32b = ", ui32b, " ui64b = ", ui64b, " mod(ui32b,ui64b) = ", mod(ui32b,ui64b));
writeln("ui32b = ", ui32b, " i8a = ", i8a, " mod(ui32b,i8a) = ", mod(ui32b,i8a));
writeln("ui32b = ", ui32b, " i8c = ", i8c, " mod(ui32b,i8c) = ", mod(ui32b,i8c));
writeln("ui32b = ", ui32b, " i16a = ", i16a, " mod(ui32b,i16a) = ", mod(ui32b,i16a));
writeln("ui32b = ", ui32b, " i16c = ", i16c, " mod(ui32b,i16c) = ", mod(ui32b,i16c));
writeln("ui32b = ", ui32b, " i32a = ", i32a, " mod(ui32b,i32a) = ", mod(ui32b,i32a));
writeln("ui32b = ", ui32b, " i32c = ", i32c, " mod(ui32b,i32c) = ", mod(ui32b,i32c));
writeln("ui32b = ", ui32b, " i64a = ", i64a, " mod(ui32b,i64a) = ", mod(ui32b,i64a));
writeln("ui32b = ", ui32b, " i64c = ", i64c, " mod(ui32b,i64c) = ", mod(ui32b,i64c));

//writeln("ui64b = ", ui64b, " ui8b = ", ui8b, " mod(ui64b,ui8b) = ", mod(ui64b,ui8b));
//writeln("ui64b = ", ui64b, " ui16b = ", ui16b, " mod(ui64b,ui16b) = ", mod(ui64b,ui16b));
//writeln("ui64b = ", ui64b, " ui32b = ", ui32b, " mod(ui64b,ui32b) = ", mod(ui64b,ui32b));
//writeln("ui64b = ", ui64b, " ui64b = ", ui64b, " mod(ui64b,ui64b) = ", mod(ui64b,ui64b));
//writeln("ui64b = ", ui64b, " i8a = ", i8a, " mod(ui64b,i8a) = ", mod(ui64b,i8a));
//writeln("ui64b = ", ui64b, " i8c = ", i8c, " mod(ui64b,i8c) = ", mod(ui64b,i8c));
//writeln("ui64b = ", ui64b, " i16a = ", i16a, " mod(ui64b,i16a) = ", mod(ui64b,i16a));
//writeln("ui64b = ", ui64b, " i16c = ", i16c, " mod(ui64b,i16c) = ", mod(ui64b,i16c));
//writeln("ui64b = ", ui64b, " i32a = ", i32a, " mod(ui64b,i32a) = ", mod(ui64b,i32a));
//writeln("ui64b = ", ui64b, " i32c = ", i32c, " mod(ui64b,i32c) = ", mod(ui64b,i32c));
//writeln("ui64b = ", ui64b, " i64a = ", i64a, " mod(ui64b,i64a) = ", mod(ui64b,i64a));
//writeln("ui64b = ", ui64b, " i64c = ", i64c, " mod(ui64b,i64c) = ", mod(ui64b,i64c));

writeln("i8a = ", i8a, " ui8b = ", ui8b, " mod(i8a,ui8b) = ", mod(i8a,ui8b));
writeln("i8a = ", i8a, " ui16b = ", ui16b, " mod(i8a,ui16b) = ", mod(i8a,ui16b));
writeln("i8a = ", i8a, " ui32b = ", ui32b, " mod(i8a,ui32b) = ", mod(i8a,ui32b));
//writeln("i8a = ", i8a, " ui64b = ", ui64b, " mod(i8a,ui64b) = ", mod(i8a,ui64b));
writeln("i8a = ", i8a, " i8a = ", i8a, " mod(i8a,i8a) = ", mod(i8a,i8a));
writeln("i8a = ", i8a, " i8c = ", i8c, " mod(i8a,i8c) = ", mod(i8a,i8c));
writeln("i8a = ", i8a, " i16a = ", i16a, " mod(i8a,i16a) = ", mod(i8a,i16a));
writeln("i8a = ", i8a, " i16c = ", i16c, " mod(i8a,i16c) = ", mod(i8a,i16c));
writeln("i8a = ", i8a, " i32a = ", i32a, " mod(i8a,i32a) = ", mod(i8a,i32a));
writeln("i8a = ", i8a, " i32c = ", i32c, " mod(i8a,i32c) = ", mod(i8a,i32c));
writeln("i8a = ", i8a, " i64a = ", i64a, " mod(i8a,i64a) = ", mod(i8a,i64a));
writeln("i8a = ", i8a, " i64c = ", i64c, " mod(i8a,i64c) = ", mod(i8a,i64c));

writeln("i8c = ", i8c, " ui8b = ", ui8b, " mod(i8c,ui8b) = ", mod(i8c,ui8b));
writeln("i8c = ", i8c, " ui16b = ", ui16b, " mod(i8c,ui16b) = ", mod(i8c,ui16b));
writeln("i8c = ", i8c, " ui32b = ", ui32b, " mod(i8c,ui32b) = ", mod(i8c,ui32b));
//writeln("i8c = ", i8c, " ui64b = ", ui64b, " mod(i8c,ui64b) = ", mod(i8c,ui64b));
writeln("i8c = ", i8c, " i8a = ", i8a, " mod(i8c,i8a) = ", mod(i8c,i8a));
writeln("i8c = ", i8c, " i8c = ", i8c, " mod(i8c,i8c) = ", mod(i8c,i8c));
writeln("i8c = ", i8c, " i16a = ", i16a, " mod(i8c,i16a) = ", mod(i8c,i16a));
writeln("i8c = ", i8c, " i16c = ", i16c, " mod(i8c,i16c) = ", mod(i8c,i16c));
writeln("i8c = ", i8c, " i32a = ", i32a, " mod(i8c,i32a) = ", mod(i8c,i32a));
writeln("i8c = ", i8c, " i32c = ", i32c, " mod(i8c,i32c) = ", mod(i8c,i32c));
writeln("i8c = ", i8c, " i64a = ", i64a, " mod(i8c,i64a) = ", mod(i8c,i64a));
writeln("i8c = ", i8c, " i64c = ", i64c, " mod(i8c,i64c) = ", mod(i8c,i64c));

writeln("i16a = ", i16a, " ui8b = ", ui8b, " mod(i16a,ui8b) = ", mod(i16a,ui8b));
writeln("i16a = ", i16a, " ui16b = ", ui16b, " mod(i16a,ui16b) = ", mod(i16a,ui16b));
writeln("i16a = ", i16a, " ui32b = ", ui32b, " mod(i16a,ui32b) = ", mod(i16a,ui32b));
//writeln("i16a = ", i16a, " ui64b = ", ui64b, " mod(i16a,ui64b) = ", mod(i16a,ui64b));
writeln("i16a = ", i16a, " i8a = ", i8a, " mod(i16a,i8a) = ", mod(i16a,i8a));
writeln("i16a = ", i16a, " i8c = ", i8c, " mod(i16a,i8c) = ", mod(i16a,i8c));
writeln("i16a = ", i16a, " i16a = ", i16a, " mod(i16a,i16a) = ", mod(i16a,i16a));
writeln("i16a = ", i16a, " i16c = ", i16c, " mod(i16a,i16c) = ", mod(i16a,i16c));
writeln("i16a = ", i16a, " i32a = ", i32a, " mod(i16a,i32a) = ", mod(i16a,i32a));
writeln("i16a = ", i16a, " i32c = ", i32c, " mod(i16a,i32c) = ", mod(i16a,i32c));
writeln("i16a = ", i16a, " i64a = ", i64a, " mod(i16a,i64a) = ", mod(i16a,i64a));
writeln("i16a = ", i16a, " i64c = ", i64c, " mod(i16a,i64c) = ", mod(i16a,i64c));

writeln("i16c = ", i16c, " ui8b = ", ui8b, " mod(i16c,ui8b) = ", mod(i16c,ui8b));
writeln("i16c = ", i16c, " ui16b = ", ui16b, " mod(i16c,ui16b) = ", mod(i16c,ui16b));
writeln("i16c = ", i16c, " ui32b = ", ui32b, " mod(i16c,ui32b) = ", mod(i16c,ui32b));
//writeln("i16c = ", i16c, " ui64b = ", ui64b, " mod(i16c,ui64b) = ", mod(i16c,ui64b));
writeln("i16c = ", i16c, " i8a = ", i8a, " mod(i16c,i8a) = ", mod(i16c,i8a));
writeln("i16c = ", i16c, " i8c = ", i8c, " mod(i16c,i8c) = ", mod(i16c,i8c));
writeln("i16c = ", i16c, " i16a = ", i16a, " mod(i16c,i16a) = ", mod(i16c,i16a));
writeln("i16c = ", i16c, " i16c = ", i16c, " mod(i16c,i16c) = ", mod(i16c,i16c));
writeln("i16c = ", i16c, " i32a = ", i32a, " mod(i16c,i32a) = ", mod(i16c,i32a));
writeln("i16c = ", i16c, " i32c = ", i32c, " mod(i16c,i32c) = ", mod(i16c,i32c));
writeln("i16c = ", i16c, " i64a = ", i64a, " mod(i16c,i64a) = ", mod(i16c,i64a));
writeln("i16c = ", i16c, " i64c = ", i64c, " mod(i16c,i64c) = ", mod(i16c,i64c));

writeln("i32a = ", i32a, " ui8b = ", ui8b, " mod(i32a,ui8b) = ", mod(i32a,ui8b));
writeln("i32a = ", i32a, " ui16b = ", ui16b, " mod(i32a,ui16b) = ", mod(i32a,ui16b));
writeln("i32a = ", i32a, " ui32b = ", ui32b, " mod(i32a,ui32b) = ", mod(i32a,ui32b));
//writeln("i32a = ", i32a, " ui64b = ", ui64b, " mod(i32a,ui64b) = ", mod(i32a,ui64b));
writeln("i32a = ", i32a, " i8a = ", i8a, " mod(i32a,i8a) = ", mod(i32a,i8a));
writeln("i32a = ", i32a, " i8c = ", i8c, " mod(i32a,i8c) = ", mod(i32a,i8c));
writeln("i32a = ", i32a, " i16a = ", i16a, " mod(i32a,i16a) = ", mod(i32a,i16a));
writeln("i32a = ", i32a, " i16c = ", i16c, " mod(i32a,i16c) = ", mod(i32a,i16c));
writeln("i32a = ", i32a, " i32a = ", i32a, " mod(i32a,i32a) = ", mod(i32a,i32a));
writeln("i32a = ", i32a, " i32c = ", i32c, " mod(i32a,i32c) = ", mod(i32a,i32c));
writeln("i32a = ", i32a, " i64a = ", i64a, " mod(i32a,i64a) = ", mod(i32a,i64a));
writeln("i32a = ", i32a, " i64c = ", i64c, " mod(i32a,i64c) = ", mod(i32a,i64c));

writeln("i32c = ", i32c, " ui8b = ", ui8b, " mod(i32c,ui8b) = ", mod(i32c,ui8b));
writeln("i32c = ", i32c, " ui16b = ", ui16b, " mod(i32c,ui16b) = ", mod(i32c,ui16b));
writeln("i32c = ", i32c, " ui32b = ", ui32b, " mod(i32c,ui32b) = ", mod(i32c,ui32b));
//writeln("i32c = ", i32c, " ui64b = ", ui64b, " mod(i32c,ui64b) = ", mod(i32c,ui64b));
writeln("i32c = ", i32c, " i8a = ", i8a, " mod(i32c,i8a) = ", mod(i32c,i8a));
writeln("i32c = ", i32c, " i8c = ", i8c, " mod(i32c,i8c) = ", mod(i32c,i8c));
writeln("i32c = ", i32c, " i16a = ", i16a, " mod(i32c,i16a) = ", mod(i32c,i16a));
writeln("i32c = ", i32c, " i16c = ", i16c, " mod(i32c,i16c) = ", mod(i32c,i16c));
writeln("i32c = ", i32c, " i32a = ", i32a, " mod(i32c,i32a) = ", mod(i32c,i32a));
writeln("i32c = ", i32c, " i32c = ", i32c, " mod(i32c,i32c) = ", mod(i32c,i32c));
writeln("i32c = ", i32c, " i64a = ", i64a, " mod(i32c,i64a) = ", mod(i32c,i64a));
writeln("i32c = ", i32c, " i64c = ", i64c, " mod(i32c,i64c) = ", mod(i32c,i64c));

writeln("i64a = ", i64a, " ui8b = ", ui8b, " mod(i64a,ui8b) = ", mod(i64a,ui8b));
writeln("i64a = ", i64a, " ui16b = ", ui16b, " mod(i64a,ui16b) = ", mod(i64a,ui16b));
writeln("i64a = ", i64a, " ui32b = ", ui32b, " mod(i64a,ui32b) = ", mod(i64a,ui32b));
//writeln("i64a = ", i64a, " ui64b = ", ui64b, " mod(i64a,ui64b) = ", mod(i64a,ui64b));
writeln("i64a = ", i64a, " i8a = ", i8a, " mod(i64a,i8a) = ", mod(i64a,i8a));
writeln("i64a = ", i64a, " i8c = ", i8c, " mod(i64a,i8c) = ", mod(i64a,i8c));
writeln("i64a = ", i64a, " i16a = ", i16a, " mod(i64a,i16a) = ", mod(i64a,i16a));
writeln("i64a = ", i64a, " i16c = ", i16c, " mod(i64a,i16c) = ", mod(i64a,i16c));
writeln("i64a = ", i64a, " i32a = ", i32a, " mod(i64a,i32a) = ", mod(i64a,i32a));
writeln("i64a = ", i64a, " i32c = ", i32c, " mod(i64a,i32c) = ", mod(i64a,i32c));
writeln("i64a = ", i64a, " i64a = ", i64a, " mod(i64a,i64a) = ", mod(i64a,i64a));
writeln("i64a = ", i64a, " i64c = ", i64c, " mod(i64a,i64c) = ", mod(i64a,i64c));

writeln("i64c = ", i64c, " ui8b = ", ui8b, " mod(i64c,ui8b) = ", mod(i64c,ui8b));
writeln("i64c = ", i64c, " ui16b = ", ui16b, " mod(i64c,ui16b) = ", mod(i64c,ui16b));
writeln("i64c = ", i64c, " ui32b = ", ui32b, " mod(i64c,ui32b) = ", mod(i64c,ui32b));
//writeln("i64c = ", i64c, " ui64b = ", ui64b, " mod(i64c,ui64b) = ", mod(i64c,ui64b));
writeln("i64c = ", i64c, " i8a = ", i8a, " mod(i64c,i8a) = ", mod(i64c,i8a));
writeln("i64c = ", i64c, " i8c = ", i8c, " mod(i64c,i8c) = ", mod(i64c,i8c));
writeln("i64c = ", i64c, " i16a = ", i16a, " mod(i64c,i16a) = ", mod(i64c,i16a));
writeln("i64c = ", i64c, " i16c = ", i16c, " mod(i64c,i16c) = ", mod(i64c,i16c));
writeln("i64c = ", i64c, " i32a = ", i32a, " mod(i64c,i32a) = ", mod(i64c,i32a));
writeln("i64c = ", i64c, " i32c = ", i32c, " mod(i64c,i32c) = ", mod(i64c,i32c));
writeln("i64c = ", i64c, " i64a = ", i64a, " mod(i64c,i64a) = ", mod(i64c,i64a));
writeln("i64c = ", i64c, " i64c = ", i64c, " mod(i64c,i64c) = ", mod(i64c,i64c));



var re32a: real(32) = -65.3338667:real(32);
var re32c: real(32) = 81.4420537:real(32);

var re64a: real(64) = -9.88837303;
var re64c: real(64) = 1.53876293;


writeln("re32a = ", re32a, " re32a = ", re32a, " mod(re32a,re32a) = ", mod(re32a, re32a));
writeln("re32a = ", re32a, " re32c = ", re32c, " mod(re32a,re32c) = ", mod(re32a, re32c));
writeln("re32a = ", re32a, " re64a = ", re64a, " mod(re32a,re64a) = ", mod(re32a, re64a));
writeln("re32a = ", re32a, " re64c = ", re64c, " mod(re32a,re64c) = ", mod(re32a, re64c));

writeln("re32c = ", re32c, " re32a = ", re32a, " mod(re32c,re32a) = ", mod(re32c, re32a));
writeln("re32c = ", re32c, " re32c = ", re32c, " mod(re32c,re32c) = ", mod(re32c, re32c));
writeln("re32c = ", re32c, " re64a = ", re64a, " mod(re32c,re64a) = ", mod(re32c, re64a));
writeln("re32c = ", re32c, " re64c = ", re64c, " mod(re32c,re64c) = ", mod(re32c, re64c));

writeln("re64a = ", re64a, " re32a = ", re32a, " mod(re64a,re32a) = ", mod(re64a, re32a));
writeln("re64a = ", re64a, " re32c = ", re32c, " mod(re64a,re32c) = ", mod(re64a, re32c));
writeln("re64a = ", re64a, " re64a = ", re64a, " mod(re64a,re64a) = ", mod(re64a, re64a));
writeln("re64a = ", re64a, " re64c = ", re64c, " mod(re64a,re64c) = ", mod(re64a, re64c));

writeln("re64c = ", re64c, " re32a = ", re32a, " mod(re64c,re32a) = ", mod(re64c, re32a));
writeln("re64c = ", re64c, " re32c = ", re32c, " mod(re64c,re32c) = ", mod(re64c, re32c));
writeln("re64c = ", re64c, " re64a = ", re64a, " mod(re64c,re64a) = ", mod(re64c, re64a));
writeln("re64c = ", re64c, " re64c = ", re64c, " mod(re64c,re64c) = ", mod(re64c, re64c));


writeln("re64a = ", re64a, " re64a = ", re64a, " mod(re64a,re64a) = ", mod(re64a,re64a));
writeln("re64a = ", re64a, " re64c = ", re64c, " mod(re64a,re64c) = ", mod(re64a,re64c));
writeln("re64a = ", re64a, " ui64b = ", ui64b, " mod(re64a,ui64b) = ", mod(re64a,ui64b));
writeln("re64a = ", re64a, " i64a = ", i64a, " mod(re64a,i64a) = ", mod(re64a,i64a));
writeln("re64a = ", re64a, " i64c = ", i64c, " mod(re64a,i64c) = ", mod(re64a,i64c));

writeln("re64c = ", re64c, " re64a = ", re64a, " mod(re64c,re64a) = ", mod(re64c,re64a));
writeln("re64c = ", re64c, " re64c = ", re64c, " mod(re64c,re64c) = ", mod(re64c,re64c));
writeln("re64c = ", re64c, " ui64b = ", ui64b, " mod(re64c,ui64b) = ", mod(re64c,ui64b));
writeln("re64c = ", re64c, " i64a = ", i64a, " mod(re64c,i64a) = ", mod(re64c,i64a));
writeln("re64c = ", re64c, " i64c = ", i64c, " mod(re64c,i64c) = ", mod(re64c,i64c));

writeln("ui64b = ", ui64b, " re64a = ", re64a, " mod(ui64b,re64a) = ", mod(ui64b,re64a));
writeln("ui64b = ", ui64b, " re64c = ", re64c, " mod(ui64b,re64c) = ", mod(ui64b,re64c));
//writeln("ui64b = ", ui64b, " ui64b = ", ui64b, " mod(ui64b,ui64b) = ", mod(ui64b,ui64b));
//writeln("ui64b = ", ui64b, " i64a = ", i64a, " mod(ui64b,i64a) = ", mod(ui64b,i64a));
//writeln("ui64b = ", ui64b, " i64c = ", i64c, " mod(ui64b,i64c) = ", mod(ui64b,i64c));

writeln("i64a = ", i64a, " re64a = ", re64a, " mod(i64a,re64a) = ", mod(i64a,re64a));
writeln("i64a = ", i64a, " re64c = ", re64c, " mod(i64a,re64c) = ", mod(i64a,re64c));
//writeln("i64a = ", i64a, " ui64b = ", ui64b, " mod(i64a,ui64b) = ", mod(i64a,ui64b));
writeln("i64a = ", i64a, " i64a = ", i64a, " mod(i64a,i64a) = ", mod(i64a,i64a));
writeln("i64a = ", i64a, " i64c = ", i64c, " mod(i64a,i64c) = ", mod(i64a,i64c));

writeln("i64c = ", i64c, " re64a = ", re64a, " mod(i64c,re64a) = ", mod(i64c,re64a));
writeln("i64c = ", i64c, " re64c = ", re64c, " mod(i64c,re64c) = ", mod(i64c,re64c));
//writeln("i64c = ", i64c, " ui64b = ", ui64b, " mod(i64c,ui64b) = ", mod(i64c,ui64b));
writeln("i64c = ", i64c, " i64a = ", i64a, " mod(i64c,i64a) = ", mod(i64c,i64a));
writeln("i64c = ", i64c, " i64c = ", i64c, " mod(i64c,i64c) = ", mod(i64c,i64c));
