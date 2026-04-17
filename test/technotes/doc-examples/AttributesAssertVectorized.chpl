//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

proc test() {
  var A: [0..99] int;
  /* START_EXAMPLE_0 */
  @llvm.assertVectorized()
  foreach a in A { a += 1; }; // warns if this is not vectorizable (but this example should be)
  /* STOP_EXAMPLE_0 */
}
test();
