class C {
  type t;
  var x: t;
  def foo() {
    for i in x do
      writeln(i);
  }
}

type t = [1..3] int;

var c = C(t);
writeln(c);
c.foo();
