use OwnedObject;

class C {
  proc deinit() {
    writeln("Destroying C");
  }
}

proc foo() {
  var x = new Owned(new C());

  var y = x;
}


foo();

