record foo {
  type t;
  var x : t;
  function print() {
    writeln(x);
  }
}

var f : foo(t=integer);

f.x = 2;

f.print();

var g : foo(t=string);

g.x = "hello world";

g.print();
