class MyClass {
  var x:int;
  proc init() {
    x = 1;
  }
  proc deinit() {
    writeln("deinit");
  }
}

proc (owned MyClass).foo() {
  writeln("in foo");
}

proc _owned.bar() {
  writeln("in bar");
}

proc test1() {
  var x = new owned MyClass();
  writeln("in test1, calling x.foo()");
  x.foo();
  writeln("back in test1 ", x);
}

proc test2() {
  var x = new owned MyClass();
  writeln("in test2, calling x.bar()");
  x.bar();
  writeln("back in test2 ", x);
}

proc main() {
  writeln("calling test1");
  test1();
  writeln("back in main");
  writeln("calling test2");
  test2();
  writeln("back in main");
}
