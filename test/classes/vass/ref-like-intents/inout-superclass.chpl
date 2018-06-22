class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procInoutD(inout arg: unmanaged D) {
  arg.procInD(); // would be undefined if the error were not reported
}

var c = new unmanaged C();
var d = new unmanaged D();

procInoutD(c); // error
procInoutD(d); // OK
