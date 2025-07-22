//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

proc test() {
  var A: [0..99] int;
  /* START_EXAMPLE_0 */
  @llvm.assertVectorized()
  foreach a in A { /* ... */ }; // warns if this is not vectorizable
  /* STOP_EXAMPLE_0 */
}
test();
