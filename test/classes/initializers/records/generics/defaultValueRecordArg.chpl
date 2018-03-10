record Foo {
  var x;

  proc init(xVal) where !xVal: Foo {
    x = xVal;
  }
}

class DefaultArg {
  var y;

  proc init(yVal = new Foo(3)) {
    y = yVal;
    super.init();
  }
}

var c = new DefaultArg();
writeln(c);
delete c;
