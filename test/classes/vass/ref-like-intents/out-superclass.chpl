class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procOutD(out arg: D) {
  arg = new C();
}

var c: C;
var d: D;

procOutD(c); // OK
procOutD(d); // error
d.procInD(); // would be undefined if the error were not reported
