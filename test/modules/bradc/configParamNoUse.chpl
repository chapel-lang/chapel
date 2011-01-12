module M1 {
  config param debug: bool = false;

  def foo() {
    if debug then
      writeln("Debug on!");
    writeln("In M1.foo()");
  }
}

module M2 {
  def main() {
    M1.foo();
  }
}
