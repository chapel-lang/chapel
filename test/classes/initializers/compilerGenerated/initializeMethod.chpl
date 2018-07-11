// Verify the behavior of initialize methods in a default initializer world
// (we want to treat them as though a constructor was defined, then eventually
// remove them)
class Foo {
  var x: int;
  var y: bool;

  proc initialize() {
    if (y) {
      writeln(x);
    } else {
      writeln(-x);
    }
  }
}

var foo1 = new shared Foo(3, true);
var foo2 = new shared Foo(5, false);

writeln(foo1);
writeln(foo2);
