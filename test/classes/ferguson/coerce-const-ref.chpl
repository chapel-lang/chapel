class MyClass {
  var x:int;
}
class SubClass : MyClass {
  var y:int;
}

var global = new unmanaged SubClass(1,2);

proc test(const ref arg:unmanaged MyClass) {
  writeln(arg);
  global = new unmanaged SubClass(3,4);
  writeln(arg); // arg should change
}

test(global);
