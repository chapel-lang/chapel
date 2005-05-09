class foo {
  var x;
  var y;
  var z : integer = 12;
  function print() {
    writeln("x is ", x, " and y is ", y, " and z is ", z);
  }
}

var f : foo = foo(x = 2, y = 3.0);

f.print();

var f2 : foo = foo(x = "bar", y = "none");

f2.print();

var f3 : foo = foo(x = "bar", y = "all", z = 3);

f3.print();

var f4 : foo = foo(x = "bar", y = 1, z = 3);

f4.print();
