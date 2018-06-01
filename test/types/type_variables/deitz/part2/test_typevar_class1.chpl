class foo {
  type t;

  var  x : t;

  proc print() {
    writeln(x);
  }
}

var f : unmanaged foo(t=int) = new unmanaged foo(t=int);

f.x = 2;

f.print();

delete f;
