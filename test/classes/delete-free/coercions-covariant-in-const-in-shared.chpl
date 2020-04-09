

class MyClass {
  var x:int;
}

class SubClass : MyClass {
  var y:int;
}


proc acceptSharedMyClass4(const in arg:shared MyClass) {
  writeln(arg);
}
proc acceptSharedMyClass5(in arg:shared MyClass) {
  writeln(arg);
}

proc test4() {
  var instance = new shared SubClass(4,4);
  acceptSharedMyClass4(instance);
  writeln("still have ", instance);
}

proc test5() {
  var instance = new shared SubClass(5,5);
  acceptSharedMyClass5(instance);
  writeln("still have ", instance);
}

test4();
test5();

