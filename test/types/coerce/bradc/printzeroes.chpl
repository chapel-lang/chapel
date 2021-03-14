var f32: real(32) = 1.0:real(32);
var f64: real(64) = 2.0;
var fdf: real     = 3.0;

var c64: complex(64) = (4.0 + 5.0i):complex(64);
var c128: complex(128) = 6.0 + 7.0i;
var cdf: complex     = 8.0 + 9.0i;

var s: string;

s = f32:string;
writeln("s is: ", s);

s = f64:string;
writeln("s is: ", s);

s = fdf:string;
writeln("s is: ", s);

s = c64:string;
writeln("s is: ", s);

s = c128:string;
writeln("s is: ", s);

s = cdf:string;
writeln("s is: ", s);
