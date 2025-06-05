class C {
}

class D: C {
  override proc postinit() throws {
    writeln("In D.postinit()");
  }
}

var myD = new D();
