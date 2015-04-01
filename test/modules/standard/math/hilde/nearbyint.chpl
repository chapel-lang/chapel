// nearbyint.chpl
// 
// Test the nearbyint function.
//

var re32a: real(32) = -65333.8667:real(32);
var re32b: real(32) = -0.0:real(32);
var re32c: real(32) = 0.0:real(32);
var re32d: real(32) = 814.420537:real(32);

var re64a: real(64) = -9.88837303;
var re64b: real(64) = -0.0;
var re64c: real(64) = 0.0;
var re64d: real(64) = 15.3876293;


writeln("re32a = ", re32a, " nearbyint(re32a) = ", nearbyint(re32a));
writeln("re32b = ", re32b, " nearbyint(re32b) = ", nearbyint(re32b));
writeln("re32c = ", re32c, " nearbyint(re32c) = ", nearbyint(re32c));
writeln("re32d = ", re32d, " nearbyint(re32d) = ", nearbyint(re32d));

writeln("re64a = ", re64a, " nearbyint(re64a) = ", nearbyint(re64a));
writeln("re64b = ", re64b, " nearbyint(re64b) = ", nearbyint(re64b));
writeln("re64c = ", re64c, " nearbyint(re64c) = ", nearbyint(re64c));
writeln("re64d = ", re64d, " nearbyint(re64d) = ", nearbyint(re64d));

