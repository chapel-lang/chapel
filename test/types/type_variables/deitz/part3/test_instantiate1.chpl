class foo {
  type t;
  var x : t;
  proc print() {
    writeln(x);
  }
}

class bar {
  var x : int;
  var y : int;
}

var f : foo(t=bar) = new foo(t=bar);

writeln(f);
f.print();
