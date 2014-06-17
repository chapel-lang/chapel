class C {
  type t;
  var x: t;
  proc foo() {
    for i in x do
      writeln(i);
  }
}

type t = [1..3] int;

var c = new C(t);
writeln(c);
c.foo();

delete c;
