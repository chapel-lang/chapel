use OwnedObject;

class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}

proc test1() {
  var instance = new MyClass(1);
}

proc test2() {
  var instance:MyClass = new Owned(new MyClass(1));
}

proc acceptMyClass(c:MyClass) {
  writeln(c);
}

proc test3() {
  var instance = new Owned(new MyClass(1));

  acceptMyClass(instance);
}

proc test5() {
  var instance = new Owned(new SubClass(1, 2));

  var otherInstance: Owned(MyClass) = instance;

  acceptMyClass(otherInstance);
}



test1();
test2();
test3();
test5();
