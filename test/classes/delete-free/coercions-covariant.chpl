use OwnedObject;

class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}


proc acceptOwnedMyClass1(own:Owned(MyClass)) {
  writeln(own);
}
proc acceptOwnedMyClass3(const own:Owned(MyClass)) {
  writeln(own);
}

proc test1() {
  var instance = new Owned(new SubClass(1,1));
  acceptOwnedMyClass1(instance);
  writeln(instance);
}

proc test3() {
  var instance = new Owned(new SubClass(3,3));
  acceptOwnedMyClass3(instance);
  writeln(instance);
}

test1();
test3();

