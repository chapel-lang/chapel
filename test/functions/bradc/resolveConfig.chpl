module A {
  use B;
  config const n = 5;

  proc main() {
    writeln("arr is: ", arr);
  }
}

module B {
  use A;

  const arr: [1..n] int;
}
