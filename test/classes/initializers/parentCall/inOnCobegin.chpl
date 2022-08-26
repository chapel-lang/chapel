class Foo {
  var x: int;

  proc init(xVal, yVal) {
    x = xVal + yVal;
    on xVal.locale {
      cobegin {
        this.complete();
        writeln("in on+cobegin, whee!");
      }
    }
  }
}

var foo = (new owned Foo(5, 2)).borrow();
writeln(foo);
