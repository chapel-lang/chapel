record B {
  type T;
  var v: T;
  var c = 0;
}

class A{

  type t;

  var b = new B(T=t, v=min(t), c=1);
}

var a = new A(t = int);

writeln("a is: ", a);

delete a;
