class foo {
  type t;
  var x : t;
  function print() {
    writeln(x);
  }
}

var f : foo(t=integer) = foo(t=integer);

var f2 : foo(t=string) = foo(t=string, x = "hello world");

f.x = 2;

f.print();

f2.print();
