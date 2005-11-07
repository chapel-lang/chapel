class foo {
  type t;
  var x : t;
  function print() {
    writeln(x);
  }
}

var f : foo(t=integer) = foo(t=integer);

f.x = 2;

f.print();
