// This test exercises attempting to create an instance that the initializer
// does not allow and assigning it into the instantiation that cannot have
// anything assigned into it.
class Foo {
  var x;

  proc init(xVal = 3) {
    x = xVal;
  }
}

var foo: borrowed Foo(string)?;
foo = (new owned Foo("blah")).borrow();
writeln(foo.type: string);
writeln(foo);
