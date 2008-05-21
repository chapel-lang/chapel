module M1 {
  use M2;
  def foo() {
    writeln("foo");
    bar(1);
  }
  def goo() {
    writeln("M1's goo");
  }
  def main() {
    foo();
    baz();
  }
}

module M2 {
  def goo() {
    writeln("M2's goo");
  }
  def bar(param p: int) {
    writeln("bar");
    goo();
  }
  def baz() {
    bar(1);
    bar(2);
    goo();
  }
}

