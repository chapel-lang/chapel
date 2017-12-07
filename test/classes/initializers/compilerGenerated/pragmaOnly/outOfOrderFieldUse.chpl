// This verifies the behavior when a default initializer is relied upon and a
// field tries to utilize a later field in its initialization.
pragma "use default init"
class Foo {
  var x = y + 1;
  var y = 3;
}

var foo = new Foo();
writeln(foo);
delete foo;
