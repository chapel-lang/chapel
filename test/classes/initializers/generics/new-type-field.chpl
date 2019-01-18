/*
   This fails.
 */
class A {
  proc init(i :int) {
  }
}

class B {
  type t = unmanaged A;
  var x : t;
  proc init() {
    x = new t(2);
  }
}

var a = new owned A(2);
var b = new owned B();

writeln("a = ", a);
writeln("b = ", b);

delete b.x; 
