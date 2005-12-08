class foo {
  type t;
  var x : t;
  constructor initialize(type t, x : t) {
    this.x = x;
  }
  function print() {
    writeln(x);
  }
}

var f : foo(t=integer) = foo(t=integer, x=3);

f.print();
