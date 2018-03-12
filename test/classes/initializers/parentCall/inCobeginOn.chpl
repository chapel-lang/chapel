class Foo {
  var x: int;

  proc init(xVal, yVal) {
    x = xVal + yVal;
    cobegin {
      on xVal.locale {
        this.initDone();
      }
      writeln("in cobegin, whee!");
    }
  }
}

var foo = new Foo(5, 2);
writeln(foo);
delete foo;
