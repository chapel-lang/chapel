class Foo {
  var x: int;

  proc init(xVal, yVal) {
    begin {
      on xVal.locale {
        super.init(xVal + yVal);
      }
    }
  }
}

var foo = new Foo(5, 2);
writeln(foo);
delete foo;
