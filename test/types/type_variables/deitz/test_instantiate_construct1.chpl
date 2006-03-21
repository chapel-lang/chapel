class foo {
  type t;
  var x : t;
  fun initialize() {
    x += 10;
  }
  fun print() {
    writeln(x);
  }
}

var f : foo(t=int) = foo(t=int, x=3);

f.print();
