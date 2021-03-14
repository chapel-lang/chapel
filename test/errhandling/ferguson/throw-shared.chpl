proc foo() throws {
  // Error
  throw new shared IllegalArgumentError("Error message.");
}

foo();
