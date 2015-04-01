// atan2.chpl
// 
// Test the atan2 function.
//

var re32a: real(32) = -0.653338667:real(32);
var re32b: real(32) = 0.0:real(32);
var re32c: real(32) = 0.814420537:real(32);

var re64a: real(64) = -0.988837303;
var re64b: real(64) = 0.0;
var re64c: real(64) = 0.153876293;


writeln("re32a = ", re32a, " re32a = ", re32a, " atan2(re32a,re32a) = ", atan2(re32a, re32a));
writeln("re32a = ", re32a, " re32b = ", re32b, " atan2(re32a,re32b) = ", atan2(re32a, re32b));
writeln("re32a = ", re32a, " re32c = ", re32c, " atan2(re32a,re32c) = ", atan2(re32a, re32c));
writeln("re32a = ", re32a, " re64a = ", re64a, " atan2(re32a,re64a) = ", atan2(re32a, re64a));
writeln("re32a = ", re32a, " re64b = ", re64b, " atan2(re32a,re64b) = ", atan2(re32a, re64b));
writeln("re32a = ", re32a, " re64c = ", re64c, " atan2(re32a,re64c) = ", atan2(re32a, re64c));

writeln("re32b = ", re32b, " re32a = ", re32a, " atan2(re32b,re32a) = ", atan2(re32b, re32a));
writeln("re32b = ", re32b, " re32b = ", re32b, " atan2(re32b,re32b) = ", atan2(re32b, re32b));
writeln("re32b = ", re32b, " re32c = ", re32c, " atan2(re32b,re32c) = ", atan2(re32b, re32c));
writeln("re32b = ", re32b, " re64a = ", re64a, " atan2(re32b,re64a) = ", atan2(re32b, re64a));
writeln("re32b = ", re32b, " re64b = ", re64b, " atan2(re32b,re64b) = ", atan2(re32b, re64b));
writeln("re32b = ", re32b, " re64c = ", re64c, " atan2(re32b,re64c) = ", atan2(re32b, re64c));

writeln("re32c = ", re32c, " re32a = ", re32a, " atan2(re32c,re32a) = ", atan2(re32c, re32a));
writeln("re32c = ", re32c, " re32b = ", re32b, " atan2(re32c,re32b) = ", atan2(re32c, re32b));
writeln("re32c = ", re32c, " re32c = ", re32c, " atan2(re32c,re32c) = ", atan2(re32c, re32c));
writeln("re32c = ", re32c, " re64a = ", re64a, " atan2(re32c,re64a) = ", atan2(re32c, re64a));
writeln("re32c = ", re32c, " re64b = ", re64b, " atan2(re32c,re64b) = ", atan2(re32c, re64b));
writeln("re32c = ", re32c, " re64c = ", re64c, " atan2(re32c,re64c) = ", atan2(re32c, re64c));

writeln("re64a = ", re64a, " re32a = ", re32a, " atan2(re64a,re32a) = ", atan2(re64a, re32a));
writeln("re64a = ", re64a, " re32b = ", re32b, " atan2(re64a,re32b) = ", atan2(re64a, re32b));
writeln("re64a = ", re64a, " re32c = ", re32c, " atan2(re64a,re32c) = ", atan2(re64a, re32c));
writeln("re64a = ", re64a, " re64a = ", re64a, " atan2(re64a,re64a) = ", atan2(re64a, re64a));
writeln("re64a = ", re64a, " re64b = ", re64b, " atan2(re64a,re64b) = ", atan2(re64a, re64b));
writeln("re64a = ", re64a, " re64c = ", re64c, " atan2(re64a,re64c) = ", atan2(re64a, re64c));

writeln("re64b = ", re64b, " re32a = ", re32a, " atan2(re64b,re32a) = ", atan2(re64b, re32a));
writeln("re64b = ", re64b, " re32b = ", re32b, " atan2(re64b,re32b) = ", atan2(re64b, re32b));
writeln("re64b = ", re64b, " re32c = ", re32c, " atan2(re64b,re32c) = ", atan2(re64b, re32c));
writeln("re64b = ", re64b, " re64a = ", re64a, " atan2(re64b,re64a) = ", atan2(re64b, re64a));
writeln("re64b = ", re64b, " re64b = ", re64b, " atan2(re64b,re64b) = ", atan2(re64b, re64b));
writeln("re64b = ", re64b, " re64c = ", re64c, " atan2(re64b,re64c) = ", atan2(re64b, re64c));

writeln("re64c = ", re64c, " re32a = ", re32a, " atan2(re64c,re32a) = ", atan2(re64c, re32a));
writeln("re64c = ", re64c, " re32b = ", re32b, " atan2(re64c,re32b) = ", atan2(re64c, re32b));
writeln("re64c = ", re64c, " re32c = ", re32c, " atan2(re64c,re32c) = ", atan2(re64c, re32c));
writeln("re64c = ", re64c, " re64a = ", re64a, " atan2(re64c,re64a) = ", atan2(re64c, re64a));
writeln("re64c = ", re64c, " re64b = ", re64b, " atan2(re64c,re64b) = ", atan2(re64c, re64b));
writeln("re64c = ", re64c, " re64c = ", re64c, " atan2(re64c,re64c) = ", atan2(re64c, re64c));

