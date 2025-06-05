class C {
  proc postinit() {
    writeln("In C.postinit()");
  }
}

class D: C {
  override proc postinit() {
    writeln("In D.postinit()");
  }
}

var myD = new D();
