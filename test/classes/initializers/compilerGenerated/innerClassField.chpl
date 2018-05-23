class Foo {
  var x: int;
}

class Bar {
  var y = new unmanaged Foo(2);
}

var b = new unmanaged Bar();
writeln(b);
delete b.y;
delete b;
