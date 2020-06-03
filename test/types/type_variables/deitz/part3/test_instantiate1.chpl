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

var f : unmanaged foo(t=borrowed bar?) = new unmanaged foo(t=borrowed bar?);

writeln(f);
f.print();

delete f;
