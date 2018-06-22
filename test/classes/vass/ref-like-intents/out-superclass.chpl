class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procOutD(out arg: unmanaged D) {
  arg = new unmanaged C();
}

var c: unmanaged C;
var d: unmanaged D;

procOutD(c); // OK
procOutD(d); // error
d.procInD(); // would be undefined if the error were not reported
