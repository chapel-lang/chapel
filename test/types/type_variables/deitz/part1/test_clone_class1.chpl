class foo {
  var x;
  proc print() {
    writeln(x);
  }
}

var f : foo(int) = new foo(2);

f.print();
