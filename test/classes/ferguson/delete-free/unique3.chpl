use UniquePtr;

class C {
  proc ~C() {
    writeln("Destroying C");
  }
}

proc foo() {
  var x:Owned(C);
  var y = new Owned(new C());

  x = y;
}


foo();

