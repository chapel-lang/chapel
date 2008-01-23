module A {
  use B;
  config const n = 5;

  def main() {
    writeln("arr is: ", arr);
  }
}

module B {
  use A;

  const arr: [1..n] int;
}
