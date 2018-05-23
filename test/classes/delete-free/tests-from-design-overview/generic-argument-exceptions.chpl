class MyClass {
  var x:int;
  proc init() {
    writeln("in MyClass.init");
  }
  proc deinit() {
    writeln("in MyClass.deinit");
  }
}

proc printType(type t) {
  writeln(t:string);
}

proc test1() {
  writeln("test1");
  printType(owned MyClass); // outputs 'owned MyClass'

  var x = new owned MyClass();
  printType(x.type); // outputs 'owned MyClass'
}
test1();

record Container {
  var field;
}

proc test2() {
  writeln("test2");
  var y = new Container(new owned MyClass());
  // y has type Container(owned MyClass)
  writeln(y.type:string);
  writeln(y.field.type:string);
}
test2();
