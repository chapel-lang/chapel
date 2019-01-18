class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}

proc test1() {
  var instance = new unmanaged MyClass(1);
  delete instance;
}

proc test2() {
  var instance:borrowed MyClass = new shared MyClass(1);
}

proc acceptMyClass(c:borrowed MyClass) {
  writeln(c);
}

proc test3() {
  var instance = new shared MyClass(1);

  acceptMyClass(instance);
}

proc test5() {
  var instance = new shared SubClass(1, 2);

  var otherInstance: shared MyClass = instance;

  acceptMyClass(otherInstance);
}



test1();
test2();
test3();
test5();
