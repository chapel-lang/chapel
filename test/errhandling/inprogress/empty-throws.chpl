proc noActualThrow() throws {
  writeln("did not throw");
}

try! noActualThrow();

writeln("normal control flow");
