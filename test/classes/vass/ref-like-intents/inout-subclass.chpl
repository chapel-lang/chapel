class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procInoutC(inout arg: unmanaged C) {
  arg = new unmanaged C();
}

var c:unmanaged C;
var d:unmanaged D;

procInoutC(c); // OK
procInoutC(d); // error
d.procInD(); // would be undefined if the error were not reported
