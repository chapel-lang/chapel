class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procOutC(out arg: C?) {
  arg = new C();
}

var c: borrowed C?;
var d: borrowed D?;

procOutC(c); // OK
procOutC(d); // error
d.procInD(); // would be undefined if the error were not reported
