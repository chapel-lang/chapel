union foo {
  var i : int;
  var f : float;
  function hello() {
    writeln("Hello");
  }
  function printf() {
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
