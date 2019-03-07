class Foo {
  var x: int;

  proc init(xVal, yVal) {
    begin {
      on xVal.locale {
        this.init(xVal + yVal);
      }
    }
  }

  proc init(xVal) {
    x = xVal;
  }
}

var foo = new unmanaged Foo(5, 2);
writeln(foo);
delete foo;
