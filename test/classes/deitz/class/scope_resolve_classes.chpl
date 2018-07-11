var y = 3;

class C {
  var x = 2;
}

module M {
  var y = 1;
  proc C.foo() {
    writeln(y, x);
  }

  proc main() {
    var c = new unmanaged C();
    c.foo();
    delete c;
  }
}
