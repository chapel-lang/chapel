var a: real = 1e+200;
var b: real = 1.123e-300;
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

var relTol1: real = 1e-8;
var relTol2: real = 1e-9;


writeln("a = ", a," e = ", e);
writeln("isClose(a,e) = ", isClose(a, e));
writeln("b = ", b," f = ", f);
writeln("isClose(b,f) = ", isClose(b, f));
writeln("d = ", d," h = ", h);
writeln("isClose(d,h) = ", isClose(d, h));

writef("i = %n l = %.10r relTol = %n\n", i, l, relTol1);
writeln("isClose(i,l,relTol) = ", isClose(i, l, relTol1));
writef("j = %n m = %.10r relTol = %n\n", j, m, relTol1);
writeln("isClose(j,m,relTol) = ", isClose(j, m, relTol1));
writef("k = %.10r n = %.10r relTol = %n\n", k, n, relTol1);
writeln("isClose(k,n,relTol) = ", isClose(k, n, relTol1));
writef("i = %n l = %.10r relTol = %n\n", i, l, relTol2);
writeln("isClose(i,l,relTol) = ", isClose(i, l, relTol2));
writef("j = %n m = %.10r relTol = %n\n", j, m, relTol2);
writeln("isClose(j,m,relTol) = ", isClose(j, m, relTol2));
writef("k = %.10r n = %.10r relTol = %n\n", k, n, relTol2);
writeln("isClose(k,n,relTol) = ", isClose(k, n, relTol2));
