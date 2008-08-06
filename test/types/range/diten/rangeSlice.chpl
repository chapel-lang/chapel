var a = 2:uint..10:uint by 2;
var b = 0:uint..10:uint by 3;
var c = 2..10 by 2;
var d = 0..10 by 3;

var e = 0..5;
var f = min(int)..max(int) by 3;
var g = 0:int(64)..5:int(64);
var h = min(int):int(64)..max(int):int(64) by 3;

writeln("uint:    ", a, "(", b, ") = ", a(b));
writeln("int:     ", c, "(", d, ") = ", c(d));
writeln("int:     ", e, "(", f, ") = ", e(f));
writeln("int(64): ", g, "(", h, ") = ", g(h));
