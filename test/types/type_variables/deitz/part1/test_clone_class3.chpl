class foo {
  var x;
  var y : int = 12;
  proc print() {
    writeln("x is ", x, " and y is ", y);
  }
}

var f1 = new unmanaged foo(x = 2);

f1.print();

var f2 = new unmanaged foo(x = "bar");

f2.print();

delete f2;
delete f1;
