class Foo {
  var x: int;

  proc init(xVal, yVal) {
    cobegin {
      on xVal.locale {
        this.init(xVal + yVal);
      }
      writeln("in cobegin, whee!");
    }
  }

  proc init(xVal) {
    x = xVal;
  }
}

var foo = (new owned Foo(5, 2)).borrow();
writeln(foo);
