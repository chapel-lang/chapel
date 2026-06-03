module M {
  proc canThrow() throws {
    throw new owned Error();
  }
  proc caller() {
    canThrow();
  }
  caller();
}
