class MyClass {
  var x: int;
  proc method() {
    writeln("in method");
  }
}

proc okSetByRef() {
  var x: owned MyClass?;
  ref r = x;
  r = new owned MyClass(1);
  x!.method();
}
okSetByRef();

proc returnRefArg(ref arg) ref {
  return arg;
}

proc okSetByObscuredRef() {
  var x: owned MyClass?;
  ref r = returnRefArg(x);
  r = new owned MyClass(1);
  x!.method();
}
okSetByObscuredRef();

config const option = true;

proc okSetByUncertainRef() {
  var x: owned MyClass?;
  var y: owned MyClass? = new owned MyClass(1);
  ref r = if option then x else y;
  r = new owned MyClass(1);
  x!.method();
}
okSetByUncertainRef();
