/*
This test verifies that updating a pragma-"private" global variable
within a coforall+on works as expected, i.e., updates the copy
of that variable that is private to that locale.

This idiom occurs in modules/internal/ChapelLocale.chpl for 'rootLocale'.
Here we verify its correctness explicitly.
*/

var   globalNonpriv: int = 777;
//proc globalOKNP() return globalNonpriv<numLocales;

pragma "private" var globalYespriv: int = 888;

proc tester() {
  coforall locIdx in 0..#numLocales with (ref globalNonpriv, ref globalYespriv) {
    extern var c_sublocid_any:chpl_sublocID_t;
    const locID = chpl_buildLocaleID(locIdx:chpl_nodeID_t, c_sublocid_any);
    on __primitive("chpl_on_locale_num", locID) {
      globalNonpriv = locIdx;
      globalYespriv = locIdx;
    }
  }
}

writeln(globalNonpriv<numLocales);

tester();

writeln(globalYespriv, " ", globalNonpriv<numLocales);

for loc in Locales do
  on loc do
    writeln(here.id, " : ", globalYespriv, " ", globalNonpriv<numLocales);

writeln("done");
