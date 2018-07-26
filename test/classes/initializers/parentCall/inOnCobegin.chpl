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

var foo = new borrowed Foo(5, 2);
writeln(foo);
