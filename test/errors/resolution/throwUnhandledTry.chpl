pragma "error mode strict"
module M {
  proc canThrow() throws {
    throw new owned Error();
  }
  proc caller() {
    try canThrow();
  }
  caller();
}
