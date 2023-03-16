// This test exercises when a generic value argument to an initializer on a
// generic class with a generic var field has a default value

class Foo {
  var v;

  proc init(vVal = 3) {
    v = vVal;
  }
}

var ownFoo1 = new owned Foo(true); // specifies a different value (of a different type)
var foo1 = ownFoo1.borrow();
var ownFoo2 = new owned Foo(); // relies on the default value
var foo2 = ownFoo2.borrow();
var ownFoo3 = new owned Foo(3); // specifies the same value as the default
var foo3 = ownFoo3.borrow();

writeln(foo1);
// We expect v to be 1 in this case, as the default value for the argument
// limits the type of v and vVal to "int", coercing when necessary.
writeln(foo2);
writeln(foo3);
