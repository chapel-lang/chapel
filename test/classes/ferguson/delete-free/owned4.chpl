use OwnedObject;

class C {
  var x = 1;
  proc deinit() {
    writeln("Destroying C ", x);
  }
}

proc foo() {
  var x:Owned(C);
  var y = new Owned(new C(2));

  x.retain(y.release());
}


foo();

