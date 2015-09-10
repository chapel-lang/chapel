class storage {
  var a: int;
}

class A {
  var stored: storage = new storage();
  var uninit: storage;

  proc ~A() {
    delete stored;
    delete uninit;
  }
}

class B: A {
  var count: int = 1;
}

var instantiated: B = new B();
writeln(instantiated);
delete instantiated;
