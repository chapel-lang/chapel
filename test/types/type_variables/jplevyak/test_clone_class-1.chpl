class foo {
  var x;
  var y : int = 12;
  fun print() {
    writeln("x is ", x, " and y is ", y);
  }
}

var f = foo(x = 2);

f.print();

var f2 = foo(x = 3.0);

f2.print();
