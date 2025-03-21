class C {
  proc postinit() throws {
    writeln("In C.postinit()");
  }
}

class D: C {
  override proc postinit() {
    writeln("In D.postinit()");
  }
}

var myD = new D();
