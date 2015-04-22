
var half : real = 0x1p-1; // 0.5
var four : real = 0x1p2; // 4.0
var _128 : real = 0x1p7; // 128.0

var one_and_half : real = 0x1.8; // 1.5 no exponent OK
var half2 : real = 0x.8; // 0.5 no exponent OK
var half3 : real = 0x0.8; // 0.5 no exponent OK

var quarter : real = 0x.4; // 0.25 no exponent OK
var eighth : real = 0x.2; // 0.125 no exponent OK

var one_sixteenth = 0x.1; // 0.0625
var one_minus_one_sixteenth = 0x.f; // 0.937500

writeln(half);
assert(half == 0.5);
writeln(four);
assert(four == 4.0);
writeln(_128);
assert(_128 == 128.0);
writeln(one_and_half);
assert(one_and_half == 1.5);
writeln(half2);
assert(half2 == 0.5);
writeln(half3);
assert(half3 == 0.5);
writeln(quarter);
assert(quarter == 0.25);
writeln(eighth);
assert(eighth == 0.125);
writeln(one_sixteenth);
assert(one_sixteenth == 0.0625);
writeln(one_minus_one_sixteenth);
assert(one_minus_one_sixteenth == 0.937500);

