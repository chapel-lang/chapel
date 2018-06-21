// This verifies the behavior when a default initializer is relied upon and a
// field tries to utilize an earlier field in its initialization.

class Foo {
  var x = 3;
  var y = x + 1;
}

var foo = new shared Foo();
writeln(foo);
