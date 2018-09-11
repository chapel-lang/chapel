class C {
  var x:int;
  proc deinit() {
    writeln("Destroying C ", x);
  }
}

proc myLegacyFunction(arg:borrowed C) {
  writeln(arg.x);
}

proc test() {
  var x = new owned C();

  myLegacyFunction(x); // feature request: coerce from Owned to C (borrow)
}

test();
