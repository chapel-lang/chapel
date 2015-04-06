// log10.chpl
// 
// Test the log10 function.
//

var re32a: real(32) = -65.3338667:real(32);
var re32b: real(32) = -1.0:real(32);
var re32c: real(32) = 10.00:real(32);
var re32d: real(32) = 8144.20537:real(32);

var re64a: real(64) = -988.837303;
var re64b: real(64) = -0.01;
var re64c: real(64) = 100000.0;
var re64d: real(64) = 150.3876293;


writeln("re32a = ", re32a, " log10(re32a) = ", log10(re32a));
writeln("re32b = ", re32b, " log10(re32b) = ", log10(re32b));
writeln("re32c = ", re32c, " log10(re32c) = ", log10(re32c));
writeln("re32d = ", re32d, " log10(re32d) = ", log10(re32d));

writeln("re64a = ", re64a, " log10(re64a) = ", log10(re64a));
writeln("re64b = ", re64b, " log10(re64b) = ", log10(re64b));
writeln("re64c = ", re64c, " log10(re64c) = ", log10(re64c));
writeln("re64d = ", re64d, " log10(re64d) = ", log10(re64d));

