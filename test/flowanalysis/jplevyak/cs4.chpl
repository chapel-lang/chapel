class C { var c; }
class D { var d; }
function makeC() { var c = new C; return c; }
function makeD() { var d = new D; return d; }
var c1 = makeC(); c1.c = 1;
var d1 = makeD(); d1.d = c1;
var c2 = makeC(); c2.c = 2.0;
var d2 = makeD(); d2.d = c2;
var d3 = makeD(); d3.d = 3.0;
var d4 = makeD(); d4.d = 4.0;
writeln(d1.d.c + d2.d.c + d3.d + d4.d);
