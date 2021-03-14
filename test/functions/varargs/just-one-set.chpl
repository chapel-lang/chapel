/*
 * It is unclear from the specification whether Chapel intends to support
 * cases in which procedures accept multiple typed var arg sets or not.
 *
 * Informal discussion imagines this might be appealing at some point but
 * the current implementation assumes that there will be at most one set.
 * This assumption is now explicit and this test confirms that a diagnostic
 * will be generated.
 */


proc main() {
  test1(1, 2, 3);

  test2(10.0, 20.0, 30.0);

  test3(1, 2, 3, 10.0, 20.0, 30.0);
}

proc test1(t1 : int ... ?N1) {
  writeln('t1: ', t1);
  writeln();
}

proc test2(t2 : real ... ?N2) {
  writeln('t2: ', t2);
  writeln();
}

proc test3(t1 : int ... ?N1, t2 : real ... ?N2) {
  writeln('t1: ', t1);
  writeln('t2: ', t2);
}
