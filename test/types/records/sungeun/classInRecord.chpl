class myC {
  var i: int;
  proc myC() { i = -1; }
}

record myR {
  var c: myC;
  proc myR() { c = new myC(); }
  proc ~myR() { delete c; }
}

pragma "auto destroy fn"
inline proc chpl__autoDestroy(x: myR) { }

proc =(a: myR, b: myR) {
  var aa: myR;
  aa.c = new myC();
  aa.c.i = b.c.i;
  return aa;
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

