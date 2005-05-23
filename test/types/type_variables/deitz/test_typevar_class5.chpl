class foo {
  type t;
  var x : t;
  var y : t;
  function print() {
    writeln(x, " and ", y);
  }
}

var f : foo = foo(t = integer);

var f2 : foo = foo(t = string, x = "hello", y = "world");

f.x = 12;
f.y = 14;

f.print();
f2.print();
