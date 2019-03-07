module Test {
  proc throwme() throws {
    throw new owned IllegalArgumentError("Bozos", "Sing");
  }

  proc test() {
    coforall i in 1..10 {
      writeln(i);
      throwme();
      try! { }
    }
  }

  test();
}
