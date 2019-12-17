class C {
  var intField: int;
}

proc test() {
  var b;
  {
    var inner = new owned C(1);
    b = inner.borrow();
  }
  writeln(b);
}
test();
