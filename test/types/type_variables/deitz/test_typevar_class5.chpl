class foo {
  type t;
  var x : t;
  var y : t;
  function print() {
    writeln(x, " and ", y);
  }
}

var f : foo(t=integer) = foo(t = integer);

var f2 : foo(t=string) = foo(t = string, x = "hello", y = "world");

f.x = 12;
f.y = 14;

f.print();
f2.print();
