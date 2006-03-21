class foo {
  type t;
  var x : t;
  fun print() {
    writeln(x);
  }
}

class bar {
  var x : int;
  var y : int;
}

var b : bar = bar(x = 12, y = 13);
writeln(b);

var f : foo(t=bar) = foo(t=bar);

writeln(f);
f.print();
