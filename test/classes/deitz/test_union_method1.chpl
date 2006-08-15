union foo {
  var i : int;
  var f : float;
  def hello() {
    writeln("Hello");
  }
}

var x : foo = foo();

x.i = -1;

writeln("x: (", x.i, ")");

x.f = 2.2;

writeln("x: (", x.f, ")");

x.hello();
