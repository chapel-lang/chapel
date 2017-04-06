// This test exercises when a generic value argument to an initializer on a
// generic class with a generic var field has a default value

class Foo {
  var v;

  proc init(vVal = 3) {
    v = vVal;
    super.init();
  }
}

var foo1 = new Foo(true); // specifies a different value (of a different type)
var foo2 = new Foo(); // relies on the default value
var foo3 = new Foo(3); // specifies the same value as the default

writeln(foo1);
// We expect v to be 1 in this case, as the default value for the argument
// limits the type of v and vVal to "int", coercing when necessary.
writeln(foo2);
writeln(foo3);

delete foo1;
delete foo2;
delete foo3;
