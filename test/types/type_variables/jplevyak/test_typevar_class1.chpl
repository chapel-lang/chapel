class foo {
  type t;
  var x : t;
  function print() {
    writeln(x);
  }
}

var f : foo(int) = foo(t=int);

f.x = 2;

f.print();
