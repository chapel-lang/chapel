class foo {
  type t;
  var x : t;
  proc postinit() {
    x += 10;
  }
  proc print() {
    writeln(x);
  }
}

var f : unmanaged foo(t=int) = new unmanaged foo(t=int, x=3);

f.print();

delete f;
