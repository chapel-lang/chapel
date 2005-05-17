class foo {
  var x;
  var y : integer = 12;
  function print() {
    writeln("x is ", x, " and y is ", y);
  }
}

var f : foo = foo(x = 2);

f.print();

var f2 : foo = foo(x = 3.0);

f2.print();
