

class MyClass {
  var x;
}

proc test1() {
  var instance = new owned MyClass(1);
}

proc test2() {
  var instance:borrowed MyClass(int) = new owned MyClass(1);
}

proc acceptMyClass(c:borrowed MyClass) {
  writeln(c);
}

proc test3() {
  var instance = new owned MyClass(1);

  acceptMyClass(instance);
}

proc test5() {
  var instance = new owned MyClass(1);

  var otherInstance: owned MyClass(int) = instance;

  acceptMyClass(otherInstance);
}

test1();
test2();
test3();
test5();
