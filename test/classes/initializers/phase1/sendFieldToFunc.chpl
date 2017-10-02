class Foo {
  var x: bool;

  proc init(xVal) {
    x = xVal;
    bar(x); // This should be allowed
    super.init();
  }
}

proc bar(val) {
  writeln(val);
  return !val;
}

var foo = new Foo(true);
writeln(foo);
delete foo;
