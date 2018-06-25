class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procOutC(out arg: unmanaged C) {
  arg = new unmanaged C();
}

var c:unmanaged C;
var d:unmanaged D;

procOutC(c); // OK
procOutC(d); // error
d.procInD(); // would be undefined if the error were not reported
