class Foo {
  var x: int;

  proc init(xVal) {
    on xVal.locale {
      cobegin {
        x = xVal;
        writeln("In a cobegin, whee!");
      }
    }
  }
}

var foo = new shared Foo(14);
writeln(foo);
