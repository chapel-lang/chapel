class bar {
  type t;
  var y : t;
}

class foo {
  type t;
  var x : bar(t) = bar(t);
  function print() {
    writeln(x.y);
  }
}

var f : foo(t=integer) = foo(t = integer);

f.x.y = 12;

f.print();
