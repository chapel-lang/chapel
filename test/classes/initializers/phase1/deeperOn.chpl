class Foo {
  var x: int;

  proc init(xVal) {
    cobegin {
      {
        on xVal.locale {
          x = xVal;
        }
      }
      writeln("In a cobegin, whee!");
    }
  }
}

var foo = new shared Foo(14);
writeln(foo);
