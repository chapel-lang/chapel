class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procRefC(ref arg: C) {
  arg = new C();
}

var c:C;
var d:D;

procRefC(c); // OK
procRefC(d); // error
d.procInD(); // would be undefined if the error were not reported
