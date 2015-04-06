// log.chpl
// 
// Test the log function.
//

var re32a: real(32) = -0.653338667:real(32);
var re32b: real(32) = -0.0:real(32);
var re32c: real(32) = 0.0:real(32);
var re32d: real(32) = 0.814420537:real(32);

var re64a: real(64) = -0.988837303;
var re64b: real(64) = -0.0;
var re64c: real(64) = 0.0;
var re64d: real(64) = 0.153876293;


writeln("re32a = ", re32a, " log(re32a) = ", log(re32a));
writeln("re32b = ", re32b, " log(re32b) = ", log(re32b));
writeln("re32c = ", re32c, " log(re32c) = ", log(re32c));
writeln("re32d = ", re32d, " log(re32d) = ", log(re32d));

writeln("re64a = ", re64a, " log(re64a) = ", log(re64a));
writeln("re64b = ", re64b, " log(re64b) = ", log(re64b));
writeln("re64c = ", re64c, " log(re64c) = ", log(re64c));
writeln("re64d = ", re64d, " log(re64d) = ", log(re64d));

