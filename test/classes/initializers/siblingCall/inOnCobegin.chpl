class Foo {
  var x: int;

  proc init(xVal, yVal) {
    on xVal.locale {
      cobegin {
        this.init(xVal + yVal);
        writeln("in on+cobegin, whee!");
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
