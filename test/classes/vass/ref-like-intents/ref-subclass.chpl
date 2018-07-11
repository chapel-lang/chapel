class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procRefC(ref arg: unmanaged C) {
  arg = new unmanaged C();
}

var c:unmanaged C;
var d:unmanaged D;

procRefC(c); // OK
procRefC(d); // error
d.procInD(); // would be undefined if the error were not reported
