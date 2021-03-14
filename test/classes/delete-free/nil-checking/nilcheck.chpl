class MyClass {
  var x: int;
  proc method() {
    writeln("in method");
  }
}

proc bad1() {
  var x:borrowed MyClass?;
  x!.method();
}
bad1();

proc bad2() {
  var x:borrowed MyClass?;
  x = nil;
  x!.method();
}
bad2();

proc bad3() {
  var x:unmanaged MyClass?;
  x!.method();
}
bad3();

proc bad4() {
  var x:unmanaged MyClass?;
  x = nil;
  x!.method();
}
bad4();

proc bad5() {
  var x:owned MyClass?;
  x!.method();
}
bad5();

proc bad6() {
  var x:owned MyClass?;
  x = nil;
  x!.method();
}
bad6();

proc bad7() {
  var x:shared MyClass?;
  x!.method();
}
bad7();

proc bad8() {
  var x:shared MyClass?;
  x = nil;
  x!.method();
}
bad8();

config param falseParam = false;

proc bad9() {
  var x:owned MyClass?;
  if falseParam then
    x = new owned MyClass(1);
  x!.method();
}
bad9();



proc ok1() {
  var x:MyClass = new borrowed MyClass(1);
  x.method();
}
ok1();

proc ok2() {
  var x:borrowed MyClass?;
  x = new borrowed MyClass(1);
  x!.method();
}
ok2();

proc ok3() {
  var x = new owned MyClass(1);
  x.method();
}
ok3();

proc ok4() {
  var x:owned MyClass?;
  x = new owned MyClass(1);
  x!.method();
}
ok4();

proc ok5() {
  var x = new shared MyClass(1);
  x.method();
}
ok5();

proc ok6() {
  var x:shared MyClass?;
  x = new shared MyClass(1);
  x!.method();
}
ok6();

proc ok7() {
  var x = new unmanaged MyClass(1);
  x.method();
}
ok7();

proc ok8() {
  var x:unmanaged MyClass?;
  x = new unmanaged MyClass(1);
  x!.method();
}
ok8();
