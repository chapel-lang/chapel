class C { var c; }
class D { var d; }
function makeC(var a) { var c = new C; c.c = a; return c; }
function makeD(var a) { var d = new D; d.d = a; return d; }
var c1 = makeC(1);
var d1 = makeD(c1);
var d3 = makeD(3.0);
writeln(d1.d.c + d3.d);
