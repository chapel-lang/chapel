class Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    on xVal {
      this.complete();
    }
  }
}

var foo = new Foo(5);
writeln(foo);
delete foo;
