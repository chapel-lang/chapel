class foo {
  var x;

  proc print() {
    writeln(x);
  }
}

var f = new unmanaged foo(3.2);

f.print();

f.x = 2;

f.print();

delete f;
