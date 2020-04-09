class Foo {
  var x;

  proc init(xVal) {
    x = xVal;
  }
}

class DefaultArg {
  var y;

  proc init(yVal = new unmanaged Foo(3)) {
    y = yVal;
  }
}

var c = new unmanaged DefaultArg();
writeln(c);
delete c.y;
delete c;
