//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

proc test0() {
  /* START_EXAMPLE_0 */
  var lastLocale = Locales[numLocales-1];
  local do on lastLocale {
    writeln("On remote locale ", lastLocale);
  }
  /* STOP_EXAMPLE_0 */
}
test0();
