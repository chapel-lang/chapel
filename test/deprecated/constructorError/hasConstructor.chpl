// TO REMOVE BEFORE 1.16
class Foo {
  var x: int;

  proc Foo(xVal) {
    x = xVal;
  }
}

var foo = new Foo(10);
writeln(foo);
delete foo;
