union foo {
  var i : integer;
  var f : float;
  function hello() {
    writeln("Hello");
  }
  function printf() {
    writeln("f value: ", f);
  }
}

var x : foo = foo();

x.f = 2.2;

writeln("x: (", x.f, ")");

x.i = -1;

writeln("x: (", x.i, ")");

x.hello();

x.printf();
