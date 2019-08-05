class C {
  var x:int;
  proc deinit() {
    writeln("Destroying C x=", x);
  }
}

proc returnByValue(in arg)
{
  return arg;
}

proc foo() {
  writeln("in foo");
  var x:owned C? = new owned C(1);
  writeln(" x=", x);

  var y = returnByValue(x);
  writeln("after returnByValue");
  writeln(" x=", x);
  writeln(" y=", y, " y type ", y.type:string);
}


foo();

proc returnByValueTyped(in arg:owned)
{
  return arg;
}

proc bar() {
  writeln("in bar");
  var x: owned C? = new owned C(1);
  writeln(" x=", x);

  var y = returnByValueTyped(x);
  writeln("after returnByValue");
  writeln(" x=", x);
  writeln(" y=", y, " y type ", y.type:string);
}


bar();
