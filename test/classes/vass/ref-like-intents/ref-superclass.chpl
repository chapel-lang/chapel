class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procRefD(ref arg: borrowed D) {
  arg.procInD(); // would be undefined if the error were not reported
}

var cObj = new C(); var c = cObj.borrow();
var dObj = new D(); var d = dObj.borrow();

procRefD(c); // error
procRefD(d); // OK
