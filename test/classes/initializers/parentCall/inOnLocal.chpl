class Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    on xVal {
      this.initDone();
    }
  }
}

var foo = new Foo(5);
writeln(foo);
delete foo;
