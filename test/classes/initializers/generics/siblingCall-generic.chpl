class Foo {
  type t;
  var x;

  proc init(xVal) {
    this.init(xVal.type, xVal);
  }

  proc init(type tVal, xVal) {
    t = tVal;
    x = xVal;
  }
}

var foo = new borrowed Foo(2);
writeln(foo.type: string);
writeln(foo);
