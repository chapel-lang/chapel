use OwnedObject;

class MyClass {
  var x;
}

proc test1() {
  var instance = new Owned(new MyClass(1));
}

proc test2() {
  var instance:MyClass(int) = new Owned(new MyClass(1));
}

proc acceptMyClass(c:MyClass) {
  writeln(c);
}

proc test3() {
  var instance = new Owned(new MyClass(1));

  acceptMyClass(instance);
}

proc test5() {
  var instance = new Owned(new MyClass(1));

  var otherInstance: Owned(MyClass(int)) = instance;

  acceptMyClass(otherInstance);
}

test1();
test2();
test3();
test5();
