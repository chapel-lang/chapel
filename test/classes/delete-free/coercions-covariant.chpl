class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}


proc acceptOwnedMyClass(own:Owned(MyClass)) {
  writeln(own);
}

proc test4() {
  var instance = new SubClass(1);

  acceptOwnedMyClass(instance);
}


test4();
