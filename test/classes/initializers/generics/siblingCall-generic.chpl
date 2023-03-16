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

var ownFoo = new owned Foo(2);
var foo = ownFoo.borrow();
writeln(foo.type: string);
writeln(foo);
