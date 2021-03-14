class foo {
  var x;
  var y;
  var z : int = 12;
  proc print() {
    writeln("x is ", x, " and y is ", y, " and z is ", z);
  }
}

var f1 = new unmanaged foo(x = 2, y = 3.0);

f1.print();

var f2 = new unmanaged foo(x = "bar", y = "none");

f2.print();

var f3 = new unmanaged foo(x = "bar", y = "all", z = 3);

f3.print();

var f4 = new unmanaged foo(x = "bar", y = "hat", z = 3);

f4.print();

delete f4;
delete f3;
delete f2;
delete f1;
