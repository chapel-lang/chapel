class foo {
  type t;
  var x : t;
  function print() {
    writeln(x);
  }
}

class bar {
  var x : int;
  var y : int;
}

var f : foo(t=bar) = foo(t=bar);

f.x = bar(x = 12, y = 13);

writeln(f);
f.print();
