class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procRefD(ref arg: borrowed D) {
  arg.procInD(); // would be undefined if the error were not reported
}

var c = (new owned C()).borrow();
var d = (new owned D()).borrow();

procRefD(c); // error
procRefD(d); // OK
