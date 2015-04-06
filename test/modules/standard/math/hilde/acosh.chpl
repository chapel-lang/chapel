// acosh.chpl
// 
// Test the acosh function.
//

var re32a: real(32) = -6.53338667:real(32);
var re32b: real(32) = -1.0:real(32);
var re32c: real(32) = 1.0:real(32);
var re32d: real(32) = 8.14420537:real(32);

var re64a: real(64) = -9.88837303;
var re64b: real(64) = -1.0;
var re64c: real(64) = 1.0;
var re64d: real(64) = 15.3876293;


writeln("re32a = ", re32a, " acosh(re32a) = ", acosh(re32a));
writeln("re32b = ", re32b, " acosh(re32b) = ", acosh(re32b));
writeln("re32c = ", re32c, " acosh(re32c) = ", acosh(re32c));
writeln("re32d = ", re32d, " acosh(re32d) = ", acosh(re32d));

writeln("re64a = ", re64a, " acosh(re64a) = ", acosh(re64a));
writeln("re64b = ", re64b, " acosh(re64b) = ", acosh(re64b));
writeln("re64c = ", re64c, " acosh(re64c) = ", acosh(re64c));
writeln("re64d = ", re64d, " acosh(re64d) = ", acosh(re64d));

