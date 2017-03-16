proc noActualThrow() throws {
  writeln("success: did not throw");
}

writeln("trying noActualThrow");
try! noActualThrow();

try! writeln("trying writeln");

writeln("success: normal control flow");
