use OwnedObject;

class MyClass {
  var x:int;
}

proc acceptRef(ref arg:MyClass) {
  var other = new Owned(new MyClass(2));
  arg = other;
}

proc test() {
  var x = new Owned(new MyClass(1));

  acceptRef(x);

  writeln(x);
}

test();
