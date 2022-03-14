try! sync {
  begin foo();
}

proc foo() throws {
  throw new Error("Intended to fail");
}
