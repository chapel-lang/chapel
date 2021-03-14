proc foo() throws {
  // Warning
  throw new unmanaged IllegalArgumentError("Error message.");
}

foo();
