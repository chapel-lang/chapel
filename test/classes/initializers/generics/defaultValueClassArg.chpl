class Foo {
  var x;

  proc init(xVal) {
    x = xVal;
    super.init();
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
