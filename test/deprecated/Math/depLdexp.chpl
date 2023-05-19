var a: real(64) = 4.0;
var b: real(32) = 3.0;

// Should be 16, but it's a real so there's variance
var check1 = ldexp(a, 2);
writeln(check1 < 17 && check1 > 15);
// Should be 24, but it's a real so there's variance
var check2 = ldexp(b, 3);
writeln(check2 < 25 && check2 > 23);
