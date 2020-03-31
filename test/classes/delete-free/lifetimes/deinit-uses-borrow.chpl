class MyClassBorrows {
  var x: int;
  var b: borrowed MyClassBorrows?;
  proc deinit() {
    writeln("in deinit, b is ", b);
  }
}

proc test2() {
  var x = new MyClassBorrows(1);
  var y = new MyClassBorrows(2);
  x.b = y.borrow();
}
test2();
