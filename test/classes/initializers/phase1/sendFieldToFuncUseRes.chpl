class Foo {
  var x: bool;
  var y: bool;

  proc init(xVal) {
    x = xVal;
    y = bar(x); // This should be allowed
  }
}

proc bar(val) {
  writeln(val);
  return !val;
}

var foo = new unmanaged Foo(true);
writeln(foo);
delete foo;
