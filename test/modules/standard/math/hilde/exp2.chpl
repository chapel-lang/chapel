// exp2.chpl
// 
// Test the exp2 function.
//

var re32a: real(32) = -0.653338667:real(32);
var re32b: real(32) = -0.0:real(32);
var re32c: real(32) = 0.0:real(32);
var re32d: real(32) = 0.814420537:real(32);

var re64a: real(64) = -0.988837303;
var re64b: real(64) = -0.0;
var re64c: real(64) = 0.0;
var re64d: real(64) = 0.153876293;


writeln("re32a = ", re32a, " exp2(re32a) = ", exp2(re32a));
writeln("re32b = ", re32b, " exp2(re32b) = ", exp2(re32b));
writeln("re32c = ", re32c, " exp2(re32c) = ", exp2(re32c));
writeln("re32d = ", re32d, " exp2(re32d) = ", exp2(re32d));

writeln("re64a = ", re64a, " exp2(re64a) = ", exp2(re64a));
writeln("re64b = ", re64b, " exp2(re64b) = ", exp2(re64b));
writeln("re64c = ", re64c, " exp2(re64c) = ", exp2(re64c));
writeln("re64d = ", re64d, " exp2(re64d) = ", exp2(re64d));

