class Foo {
  var x: int;

  proc init(xVal, yVal) {
    x = xVal + yVal;
    cobegin {
      on xVal.locale {
        init this;
      }
      writeln("in cobegin, whee!");
    }
  }
}

var foo = new shared Foo(5, 2);
writeln(foo);
