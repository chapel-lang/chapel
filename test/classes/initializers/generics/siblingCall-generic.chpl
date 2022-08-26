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

var foo = (new owned Foo(2)).borrow();
writeln(foo.type: string);
writeln(foo);
