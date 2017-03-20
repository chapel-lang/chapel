use UniquePtr;

class C {
  proc ~C() {
    writeln("Destroying C");
  }
}

proc foo() {
  var x = new Owned(new C());

  var y = x;
}


foo();

