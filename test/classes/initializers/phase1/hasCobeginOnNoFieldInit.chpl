var a, b: int;

class Foo {
  var x: int;

  proc init(xVal) {
    cobegin with (ref a, ref b) {
      on xVal.locale {
        a = xVal + 2;
      }
      b = xVal - 5;
    }

    x = xVal;
  }
}

sync {
  var foo = new unmanaged Foo(14);
  writeln(foo);
  delete foo;
}
writeln(a);
writeln(b);
