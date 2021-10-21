class Parent {
  proc whoami() {
    writeln("Parent");
  }
}

class Child: Parent {
  override proc whoami() {
    writeln("Child");
  }
}

var child = new Child();
child.super.whoami(); // Should not be allowed
