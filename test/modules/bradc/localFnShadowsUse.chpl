module M1 {
  use M2;

  def foo() {
    writeln("In M1's foo()");
  }

  def main() {
    foo();
  }
}

module M2 {
  def foo() {
    writeln("In M2's foo()");
  }
}
