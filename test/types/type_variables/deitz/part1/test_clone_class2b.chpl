class foo {
  var x;
  proc print() {
    writeln(x);
  }
}

var f = new foo(x = 2);

f.print();

var f2 = new foo(x = 3.2);

f2.print();
