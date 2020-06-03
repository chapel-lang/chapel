class C {
  var x:int;
  proc deinit() {
    writeln("Destroying C x=", x);
  }
}

proc returnByValue(ref arg)
{
  ref tmp = arg; // workaround for bug, see missing-auto-copy.chpl
  return tmp;
}


proc foo() {
  var x: owned C? = new owned C(1);
  writeln(" x=", x);

  var y = returnByValue(x);
  writeln("after returnByValue");
  writeln(" x=", x);
  writeln(" y=", y);
}


foo();

