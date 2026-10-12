//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

proc test0() {
  /* START_EXAMPLE_0 */
  // ways to use @deprecated
  @deprecated
  proc foo0() {}

  @deprecated("foo1 is deprecated, please use bar")
  proc foo1() {}

  @deprecated(since="1.30", notes="foo2 is deprecated", suggestion="use bar")
  proc foo2() {}

  // ways to use @unstable
  @unstable
  proc foo3() {}

  @unstable("foo4 is unstable")
  proc foo4() {}

  @unstable(category="experimental", issue="1234", reason="testing a new feature")
  proc foo5() {}

  // ways to use @stable
  @stable(since="1.30")
  proc foo() {}

  @stable("1.30")
  proc bar() {}
  /* STOP_EXAMPLE_0 */
}
test0();

/* START_EXAMPLE_1 */
// prevent the entire module from being documented
@chpldoc.nodoc
module M { }

// valid placement of @chpldoc.nodoc
pragma "always RVF"
@chpldoc.nodoc
proc foo() { }
/* STOP_EXAMPLE_1 */

proc test2() {
  var A: [0..99] int;
  /* START_EXAMPLE_2 */
  @llvm.metadata(
   ("llvm.loop.vectorize.enable", true), // becomes !{!"llvm.loop.vectorize.enable", i1 true}
   ("llvm.loop.vectorize.width", 4) // becomes !{!"llvm.loop.vectorize.width", i64 4}
  )
  foreach a in A { /* ... */ };
  /* STOP_EXAMPLE_2 */
}
test2();
