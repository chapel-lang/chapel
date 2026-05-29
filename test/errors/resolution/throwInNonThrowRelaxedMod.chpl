module M {
  proc canThrow() throws {
    throw new owned Error();
  }
  canThrow();
}
