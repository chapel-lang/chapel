class foo {
  var i : int;
  var f : real;
}

var y : foo = new foo(i=12,f=18.2);

writeln("y: (", y.i, ", ", y.f, ")");
