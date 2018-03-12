// This test exercises attempting to create an instance that the initializer
// does not allow and assigning it into the instantiation that cannot have
// anything assigned into it.
class Foo {
  var x;
  var y;

  proc init(xVal = 3) {
    x = xVal;
    y = xVal + 2;
  }
}

var foo: Foo(string);
foo = new Foo("blah");
writeln(foo.type: string);
writeln(foo);
