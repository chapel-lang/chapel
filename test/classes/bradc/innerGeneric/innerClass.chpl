class A {

  type t;

  class B {
    type T;
    var v: T;
    var c = 0;
  }

  var b = new B(T=t, v=min(t), c=1);

  proc ~A() {
    delete b;
  }
}

var a = new A(t = int);

writeln("a is: ", a);

delete a;
