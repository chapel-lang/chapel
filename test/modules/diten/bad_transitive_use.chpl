module M2 {
  def bar() {
    writeln("bar");
  }
}
module M1 {
  use M2;

  def foo() {
    writeln("foo");
  }
}

module M3 {
  use M1;
  def main() {
    bar();
  }
}
