use Math;

var a: real(64) = 7.0;
var b: real(32) = 7.0;

var checkL64 = lgamma(a);
var checkL32 = lgamma(b);
writeln(6.0 < checkL64 && checkL64 < 7.0);
writeln(6.0 < checkL32 && checkL32 < 7.0);
