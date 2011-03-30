class bar {
  type t;
  var y : t;
}

class foo {
  type t;
  var x : bar(t) = new bar(t);
  proc print() {
    writeln(x.y);
  }
}

var f : foo(t=int) = new foo(t = int);

f.x.y = 12;

f.print();
