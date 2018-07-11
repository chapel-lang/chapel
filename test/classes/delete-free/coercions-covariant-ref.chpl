use OwnedObject;

class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}


proc acceptOwnedMyClassB(ref own:Owned(MyClass)) {
  writeln(own);
}

proc testB() {
  var instance = new Owned(new SubClass(2,2));
  acceptOwnedMyClassB(instance);
  writeln(instance);
}

testB();
