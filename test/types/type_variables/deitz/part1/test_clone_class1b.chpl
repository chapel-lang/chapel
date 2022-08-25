class foo {
  var x;
  proc print() {
    writeln(x);
  }
}

var f = (new owned foo(2)).borrow();

f.print();

f.x = 3.2;

f.print();
