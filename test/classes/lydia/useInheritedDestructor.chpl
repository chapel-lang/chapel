class storage {
  var a: int;
}

class A {
  var stored: unmanaged storage = new unmanaged storage();
  var uninit: unmanaged storage?;

  proc deinit() {
    delete stored;
    delete uninit;
  }
}

class B: A {
  var count: int = 1;
}

var instantiated: unmanaged B = new unmanaged B();
writeln(instantiated);
delete instantiated;
