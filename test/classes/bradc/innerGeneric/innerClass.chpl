class A {

  type t;

  class B {
    type T;
    var v: T;
    var c = 0;
  }

  var b = new unmanaged B(T=t, v=min(t), c=1);

  proc deinit() {
    delete b;
  }
}

var a = new unmanaged A(t = int);

writeln("a is: ", a);

delete a;
