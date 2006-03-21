class foo {
  type t;
  var x : t;
  function initialize() {
    x += 10;
  }
  function print() {
    writeln(x);
  }
}

var f : foo(t=int) = foo(t=int, x=3);

f.print();
