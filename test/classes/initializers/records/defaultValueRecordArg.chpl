record Foo {
  var x: int;

  proc init(xVal) where xVal.type != Foo {
    x = xVal;
  }
}

class DefaultArg {
  var y: Foo;

  proc init(yVal = new Foo(3)) {
    y = yVal;
  }
}

var c = new unmanaged DefaultArg();
writeln(c);
delete c;
