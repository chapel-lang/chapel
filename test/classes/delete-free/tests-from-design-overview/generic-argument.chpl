class MyClass {
  var x:int;
  proc init() {
    writeln("in MyClass.init");
  }
  proc deinit() {
    writeln("in MyClass.deinit");
  }
}

proc f(x) {
  writeln("in f, x.type is ", x.type:string);
}

proc test1() {
  writeln("in test1");
  var x = new owned MyClass();
  f(x); // f gets a borrow
  writeln("back in test1");
  writeln(x); // Surprising if this outputs 'nil'
}
test1();

proc test2() {
  writeln("in test2");
  f(new owned MyClass()); // f gets a borrow
  writeln("back in test2");
}
test2();


proc g(x: owned) {
  writeln("in g, x.type is ", x.type:string);
}

proc test3() {
  g(new owned MyClass()); // g takes ownership
}
test3();
