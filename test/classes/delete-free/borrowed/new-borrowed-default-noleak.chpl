class MyClass {
  var x:int;
}


proc test1() {
  var ox = new owned MyClass(1);
  var x = ox.borrow();
  writeln(x);
}

proc test2() {
  var x = new MyClass(2);
  writeln(x);
}

test1();
test2();
