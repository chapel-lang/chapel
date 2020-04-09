var a, b: int;

class Foo {
  var x: int;

  proc init(xVal) {
    on xVal.locale {
      cobegin with (ref a, ref b) {
        a = xVal + 2;
        b = xVal - 5;
      }
    }

    x = xVal;
  }
}

sync {
  var foo = new owned Foo(14);
  writeln(foo);
}
writeln(a);
writeln(b);
