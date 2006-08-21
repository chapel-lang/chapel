class foo {
  type t;
  var x : t;
  def print() {
    writeln(x);
  }
}

class bar {
  var x : int;
  var y : int;
}

var f : foo(t=bar) = foo(t=bar);

writeln(f);
f.print();
