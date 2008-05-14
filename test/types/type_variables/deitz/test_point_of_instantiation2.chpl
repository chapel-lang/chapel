module M1 {
  use M2;
  def foo() {
    writeln("foo");
    bar(1);
  }
  def goo() {
    writeln("goo");
  }
  def main() {
    foo();
  }
}

module M2 {
  def bar(param p: int) {
    writeln("bar");
    goo();
  }
}

