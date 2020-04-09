class MyClass {
  var x: int;
  proc method() {
    writeln("in method");
  }
}

proc okSetInInner() {
  var x: owned MyClass?;
  proc inner() {
    x = new owned MyClass(1);
  }
  inner();
  x!.method();
}
okSetInInner();

proc okSetInTask() {
  var x: owned MyClass?;
  var s$: sync int;

  begin with (ref x) {
    x = new owned MyClass(1);
    s$ = 1;
  }

  s$; // wait for task to complete

  x!.method();
}
okSetInTask();

var global: unmanaged MyClass?;
proc setGlobal() {
  global = new unmanaged MyClass(1);
}
proc okSetGlobalInFn() {
  global = nil;
  setGlobal();
  global!.method();
  delete global;
}
okSetGlobalInFn();

proc okDeleteNil() {
  var x: unmanaged MyClass?;
  delete x;
}
okDeleteNil();
