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

var oc = new owned C();
c = oc.borrow();
d = c:borrowed D?;
if d then writeln(d); else writeln("nil value");
e = c:borrowed E?;
if e then writeln(e); else writeln("nil value");

var od = new owned D();
c = od.borrow();
d = c:borrowed D?;
if d then writeln(d); else writeln("nil value");
e = c:borrowed E?;
if e then writeln(e); else writeln("nil value");

var oe = new owned E();
c = oe.borrow();
d = c:borrowed D?;
if d then writeln(d); else writeln("nil value");
e = c:borrowed E?;
if e then writeln(e); else writeln("nil value");
