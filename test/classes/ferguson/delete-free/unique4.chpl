use UniquePtr;

class C {
  var x = 1;
  proc ~C() {
    writeln("Destroying C ", x);
  }
}

proc foo() {
  var x:Owned(C);
  var y = new Owned(new C(2));

  x.reset(y.release());
}


foo();

