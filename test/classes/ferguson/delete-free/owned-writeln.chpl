class MyClass {
  var x = 1;
  proc deinit() {
    writeln("Destroying C ", x);
  }
}

proc test() {
  var x = new owned MyClass(1);
  writeln("before writeln x=", x.borrow());
  writeln(x);
  writeln("after writeln x=", x.borrow());
}

test();
