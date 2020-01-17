var a: real = 1e+200;
var b: real = 1.123e-300;
var c: uint = 12345;
var d: real = 0.0;

var e: real = 1e+200;
var f: real = 1.123e-300;
var g: real = 12345.0;
var h: real = -0.0;


var i: real = 1e08;
var j: real = -1e-8;
var k: real = 1.12345678;

var l: real = 1e8+1;
var m: real = -1.000000009e-8;
var n: real = 1.12345679;

var rtol1: real = 1e-8;
var rtol2: real = 1e-9;


writeln("a = ", a," e = ", e);
writeln("isclose(a,e) = ", isclose(a, e));
writeln("b = ", b," f = ", f);
writeln("isclose(b,f) = ", isclose(b, f));
writeln("c = ", c," g = ", g);
writeln("isclose(c,g) = ", isclose(c, g));
writeln("d = ", d," h = ", h);
writeln("isclose(d,h) = ", isclose(d, h));

writef("i = %n l = %.10r rtol = %n\n", i, l, rtol1);
writeln("isclose(i,l,rtol) = ", isclose(i, l, rtol1));
writef("j = %n m = %.10r rtol = %n\n", j, m, rtol1);
writeln("isclose(j,m,rtol) = ", isclose(j, m, rtol1));
writef("k = %.10r n = %.10r rtol = %n\n", k, n, rtol1);
writeln("isclose(k,n,rtol) = ", isclose(k, n, rtol1));
writef("i = %n l = %.10r rtol = %n\n", i, l, rtol2);
writeln("isclose(i,l,rtol) = ", isclose(i, l, rtol2));
writef("j = %n m = %.10r rtol = %n\n", j, m, rtol2);
writeln("isclose(j,m,rtol) = ", isclose(j, m, rtol2));
writef("k = %.10r n = %.10r rtol = %n\n", k, n, rtol2);
writeln("isclose(k,n,rtol) = ", isclose(k, n, rtol2));
