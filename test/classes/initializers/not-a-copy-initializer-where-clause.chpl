// This test case is a more general version of initCallsDom.chpl, showing that
// the issue is with calling methods not defined on the type (basically, with
// assuming the generic argument will not be used as a copy initializer).

// This shows that using a where clause helps the general case (but the where
// clause doesn't help the array/domain case)
record OtherRecord {
  var someField = 10;

  proc someMethod() {
    writeln("In otherRecord's someMethod");
  }
}


record Foo {
  var a: bool;

  proc init(genericArg) where genericArg.type != Foo {
    genericArg.someMethod();
    a = true;
  }
}

var other = new OtherRecord();
var foo = new Foo(other);
writeln(foo);
