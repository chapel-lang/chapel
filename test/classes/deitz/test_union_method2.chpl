union foo {
  var i : int;
  var f : float;
  fun hello() {
    writeln("Hello");
  }
  fun printf() {
    writeln("f value: ", f);
  }
}

var x : foo = foo();

x.i = -1;

writeln("x: (", x.i, ")");

x.f = 2.2;

writeln("x: (", x.f, ")");

x.hello();

x.printf();
