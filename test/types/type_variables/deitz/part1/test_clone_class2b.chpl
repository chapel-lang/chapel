class foo {
  var x;
  proc print() {
    writeln(x);
  }
}

var f1 = new foo(x = 2);

f1.print();

var f2 = new foo(x = 3.2);

f2.print();

delete f2;
delete f1;
