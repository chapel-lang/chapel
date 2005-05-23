class bar {
  type t;
  var y : t;
}

class foo {
  type t;
  var x : bar = bar(t);
  function print() {
    writeln(x.y);
  }
}

var f : foo = foo(t = integer);

f.x.y = 12;

f.print();
