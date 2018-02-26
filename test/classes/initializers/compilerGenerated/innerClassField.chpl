class Foo {
  var x: int;
}

class Bar {
  var y = new Foo(2);
}

var b = new Bar();
writeln(b);
delete b;
