module M1 {
  use M2;

  def foo() {
    writeln("In M1's foo()");
  }

  def main() {
    M1.foo();
    M2.foo();
  }
}

module M2 {
  def foo() {
    writeln("In M2's foo()");
  }
}
