class foo {
  var x;
  proc print() {
    writeln(x);
  }
}

var f : unmanaged foo(int) = new unmanaged foo(2);

f.print();

delete f;
