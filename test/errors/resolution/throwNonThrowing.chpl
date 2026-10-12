module M {
  proc canThrow() {
    throw new owned Error();
  }
  canThrow();
}
