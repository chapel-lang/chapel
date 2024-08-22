config var computeOrRetrieve = 42;
proc f() {
  @functionStatic(computeOrRetrieve)
  ref x = 0;
}
f();
