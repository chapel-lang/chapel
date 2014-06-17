class foo {
  var x;
  var y : int = 12;
  proc print() {
    writeln("x is ", x, " and y is ", y);
  }
}

var f = new foo(x = 2);

f.print();

var f2 = new foo(x = "bar");

f2.print();
