use OwnedObject;

class C {
  var x:int;
  proc deinit() {
    writeln("Destroying C x=", x);
  }
}

proc returnByValue(arg)
{
  return arg;
}


proc foo() {
  var x = new Owned(new C(1));
  writeln(" x=", x);

  var y = returnByValue(x);
  writeln("after returnByValue");
  writeln(" x=", x);
  writeln(" y=", y);
}


foo();

