// This test case is a more general version of initCallsDom.chpl, showing that
// the issue is with calling methods not defined on the type (basically, with
// assuming the generic argument will not be used as a copy initializer).
record OtherRecord {
  var someField = 10;

  proc someMethod() {
    writeln("In otherRecord's someMethod");
  }
}


record Foo {
  var a: bool;

  // We must explicitly exclude this initializer from consideration for copy
  // initializers
  proc init(genericArg) where !isSubtype(genericArg.type, Foo) {
    genericArg.someMethod();
    a = true;
  }
}

var other = new OtherRecord();
var foo = new Foo(other);
writeln(foo);
