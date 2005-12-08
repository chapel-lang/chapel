class foo {
  type t;
  var x : t;
  function print() {
    writeln(x);
  }
}

class bar {
  var x : integer;
  var y : integer;
}

var f : foo(t=bar) = foo(t=bar);

writeln(f);
f.print();
