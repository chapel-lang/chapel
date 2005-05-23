class foo {
  type t;
  var x : t;
  function print() {
    writeln(x);
  }
}

var f : foo = foo(t=integer);

var f2 : foo = foo(t=string, x = "hello world");

f.x = 2;

f.print();

f2.print();
