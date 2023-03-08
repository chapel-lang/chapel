// Verifies we allow copy initializers to be declared as throws, regardless of
// what they contain in their body
record Foo {
  var x: int;

  proc init=(xVal: int) throws {
    x = xVal;
  }

  operator :(x: int, type t: Foo) {
    var result: Foo = new Foo(x);
    return result;
  }
}

var foo: Foo = 10;
writeln(foo);
