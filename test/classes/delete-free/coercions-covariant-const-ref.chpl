use OwnedObject;

class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}


proc acceptOwnedMyClassA(const ref own:Owned(MyClass)) {
  writeln(own);
}

proc testA() {
  var instance = new Owned(new SubClass(1,1));
  acceptOwnedMyClassA(instance);
  writeln(instance);
}

testA();
