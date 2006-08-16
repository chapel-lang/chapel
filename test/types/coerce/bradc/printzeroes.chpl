var f32: float(32) = 1.0;
var f64: float(64) = 2.0;
var fdf: float     = 3.0;

var c32: complex(32) = 4.0 + 5.0i;
var c64: complex(64) = 6.0 + 7.0i;
var cdf: complex     = 8.0 + 9.0i;

var s: string;

s = f32;
writeln("s is: ", s);

s = f64;
writeln("s is: ", s);

s = fdf;
writeln("s is: ", s);

s = c32;
writeln("s is: ", s);

s = c64;
writeln("s is: ", s);

s = cdf;
writeln("s is: ", s);
