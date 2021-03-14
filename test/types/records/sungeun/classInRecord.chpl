class myC {
  var i: int;
  proc init() { i = -1; }
}

record myR {
  var c: shared myC;
  proc init() { c = new shared myC(); }
  proc init=(other: myR) { c = other.c; }
  proc deinit() { }
}

proc =(ref a: myR, b: myR) {
  a.c = new shared myC();
  a.c.i = b.c.i;
}

var r: myR;
var r2: myR;
writeln(r);
writeln(r2);

r = new myR();
writeln(r);
writeln(r2);

r2 = r;
writeln(r);
writeln(r2);

r2.c.i = 0;
writeln(r);
writeln(r2);

foo(r);
writeln(r);
writeln(r2);

proc foo(r) {
  r.c.i = 1;
}

