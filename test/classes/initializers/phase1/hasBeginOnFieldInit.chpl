class Foo {
  var x: int;

  proc init(xVal) {
    begin {
      on xVal.locale {
        x = xVal;
      }
    }
  }
}

var foo = new Foo(14);
writeln(foo);
delete foo;
