class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}


proc acceptOwnedMyClass4(const in own:owned MyClass?) {
  writeln(own);
}
proc acceptOwnedMyClass5(in own:owned MyClass?) {
  writeln(own);
}

proc test4() {
  var instance = new owned SubClass(4,4)?;
  acceptOwnedMyClass4(instance);
  writeln("expect nil, got ", instance);
}

proc test5() {
  var instance = new owned SubClass(5,5)?;
  acceptOwnedMyClass5(instance);
  writeln("expect nil, got ", instance);
}

test4();
test5();

