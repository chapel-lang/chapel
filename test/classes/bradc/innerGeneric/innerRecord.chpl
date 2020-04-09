class A{

  type t;

  record B {
    type T;
    var v: T;
    var c = 0;
  }

  var b = new B(T=t, v=min(t), c=1);
}

var a = new unmanaged A(t = int);

writeln("a is: ", a);

delete a;
