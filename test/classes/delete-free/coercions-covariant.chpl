class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}


proc acceptOwnedMyClass1(in own:owned MyClass?) {
  writeln(own);
}
proc acceptOwnedMyClass3(const in own:owned MyClass?) {
  writeln(own);
}

proc test1() {
  var instance = new owned SubClass(1,1)?;
  acceptOwnedMyClass1(instance);
  writeln(instance);
}

proc test3() {
  var instance = new owned SubClass(3,3)?;
  acceptOwnedMyClass3(instance);
  writeln(instance);
}

test1();
test3();

