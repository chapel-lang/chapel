// This test exists to ensure that a single-statement routine does not cause
// the following comment to be dropped by the parser.

module M {
  /* doc foo */
  proc foo() do if false then ;

  /* doc bar */
  proc bar() { }
}
