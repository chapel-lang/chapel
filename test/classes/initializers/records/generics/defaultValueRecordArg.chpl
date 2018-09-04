record Foo {
  var x;

  proc init(xVal) where !isSubtype(xVal.type, Foo) {
    x = xVal;
  }
}

class DefaultArg {
  var y;

  proc init(yVal = new Foo(3)) {
    y = yVal;
  }
}

var c = new unmanaged DefaultArg();
writeln(c);
delete c;
