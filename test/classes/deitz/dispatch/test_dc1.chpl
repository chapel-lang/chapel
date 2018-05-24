class C {
  var x: int;
}

class D: C {
  var y: int;
}

class E: D {
  var z: int;
}

var c: C;
var d: D;
var e: E;

c = new borrowed C();
d = c:D;
if d then writeln(d); else writeln("nil value");
e = c:E;
if e then writeln(e); else writeln("nil value");

c = new borrowed D();
d = c:D;
if d then writeln(d); else writeln("nil value");
e = c:E;
if e then writeln(e); else writeln("nil value");

c = new borrowed E();
d = c:D;
if d then writeln(d); else writeln("nil value");
e = c:E;
if e then writeln(e); else writeln("nil value");
