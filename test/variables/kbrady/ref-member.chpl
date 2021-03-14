class Foo {
  ref x: int;
}

record Bar {
  ref x: int;
}


var y = 1;

var f = new Foo(y);
var b = new Bar(y);
writeln(f.x, b.x, y);
f.x += 1;
writeln(f.x, b.x, y);
