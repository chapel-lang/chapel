class MyClass {
  var x:int;
  proc init() {
    writeln("in MyClass.init");
  }
  proc deinit() {
    writeln("in MyClass.deinit");
  }
}

class OtherClass {
  var y:real;
  proc init() {
    writeln("in OtherClass.init");
  }
  proc deinit() {
    writeln("in OtherClass.deinit");
  }
}


proc h(x: managed?) {
  writeln("in h, x.type is ", x.type:string);
}

proc test1() {
  writeln("in test1");
  h(new owned MyClass()); // 'h' takes ownership
  writeln("back in test1");
}
test1();

proc test2() {
  writeln("in test2");
  h(new owned OtherClass()); // 'h' takes ownership
  writeln("back in test2");
}
test2();

var global: owned MyClass = new owned MyClass();
proc test3() {
  writeln("in test3");
  h(global.borrow()); // or 'h' can borrow
}
test3();

proc test4() {
  writeln("in test4");
  h(1); // 'h' can also apply to non-class things
}
test4();
