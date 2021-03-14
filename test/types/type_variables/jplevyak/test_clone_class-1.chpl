class foo {
  var x;
  var y : int = 12;
  proc print() {
    writeln("x is ", x, " and y is ", y);
  }
}



var f1 = new unmanaged foo(x = 2);

f1.print();

delete f1;




var f2 = new borrowed foo(x = 3.0);

f2.print();
