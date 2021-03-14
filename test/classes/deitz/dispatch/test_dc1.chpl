class C {
  var x: int;
}

class D: C {
  var y: int;
}

class E: D {
  var z: int;
}

var c: borrowed C?;
var d: borrowed D?;
var e: borrowed E?;

c = new borrowed C();
d = c:borrowed D?;
if d then writeln(d); else writeln("nil value");
e = c:borrowed E?;
if e then writeln(e); else writeln("nil value");

c = new borrowed D();
d = c:borrowed D?;
if d then writeln(d); else writeln("nil value");
e = c:borrowed E?;
if e then writeln(e); else writeln("nil value");

c = new borrowed E();
d = c:borrowed D?;
if d then writeln(d); else writeln("nil value");
e = c:borrowed E?;
if e then writeln(e); else writeln("nil value");
