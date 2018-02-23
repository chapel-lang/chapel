record Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    super.init();
  }
}

class DefaultArg {
  var y: Foo;

  proc init(yVal = new Foo(3)) {
    y = yVal;
    super.init();
  }
}

var c = new DefaultArg();
writeln(c);
delete c;
