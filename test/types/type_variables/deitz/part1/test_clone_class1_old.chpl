class foo {
  var x;
  proc print() {
    writeln(x);
  }
}

var f : borrowed foo(2) = (new owned foo(2)).borrow();

f.print();
