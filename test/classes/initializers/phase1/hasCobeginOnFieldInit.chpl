class Foo {
  var x: int;

  proc init(xVal) {
    cobegin {
      on xVal.locale {
        x = xVal;
      }
      writeln("In a cobegin, whee!");
    }
  }
}

var foo = new unmanaged Foo(14);
writeln(foo);
delete foo;
