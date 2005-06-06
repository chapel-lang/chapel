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

var f : foo = foo(t=bar);

f.x = bar(x = 12, y = 13);

writeln(f);
f.print();
