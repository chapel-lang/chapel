

class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}


proc acceptOwnedMyClassA(const ref own:owned MyClass) {
  writeln(own);
}

proc testA() {
  var instance = new owned SubClass(1,1);
  acceptOwnedMyClassA(instance);
  writeln(instance);
}

testA();
