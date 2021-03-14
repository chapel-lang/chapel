class MyClass {
  var x:int;
}


proc test1() {
  var x = new borrowed MyClass(1);
  writeln(x);
}

proc test2() {
  var x = new MyClass(2);
  writeln(x);
}

test1();
test2();
