

class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}


proc acceptOwnedMyClassB(ref own:owned MyClass) {
  writeln(own);
}

proc testB() {
  var instance = new owned SubClass(2,2);
  acceptOwnedMyClassB(instance);
  writeln(instance);
}

testB();
