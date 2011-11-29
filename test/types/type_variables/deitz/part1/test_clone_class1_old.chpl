class foo {
  var x;
  proc print() {
    writeln(x);
  }
}

var f : foo(2) = new foo(2);

f.print();
