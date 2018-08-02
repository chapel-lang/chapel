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

var b : borrowed bar        = new borrowed bar(x = 12, y = 13);
writeln(b);

var f : borrowed foo(t=borrowed bar) = new borrowed foo(t=borrowed bar);

writeln(f);
f.print();
