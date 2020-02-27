/*
Testing assignments into owned when r.h.s. is nil at run time.
Includes cases allowed due to holes in static checking.

Cf. the following test and its siblings test compiler errors:
 test/classes/errors/nilability-assign/assign-oknil-owned-from-oknil-owned.chpl

See also test/classes/nilability/issue-14147.chpl
*/

/* This is legal due to holes in static checking.
   When this hole is plugged, use a different hole.
   When all holes are plugged, remove altogether. */
var NILA: [1..2] owned object;

var nilableNil: owned object?;
var nonnlbLHS = new owned object();
var nonnlbLHSb = nonnlbLHS.borrow();
var nilableLHS: owned object?;
proc fillNilableLHS() { nilableLHS = new owned object(); }

proc main() {
  /* nilable <- nilable */

  writeln("1a ", nilableLHS);
  nilableLHS = nilableNil;
  writeln("1b ", nilableLHS);

  fillNilableLHS();
  writeln("2a ", nilableLHS);
  nilableLHS = nilableNil;
  writeln("2b ", nilableLHS);

  /* nilable <- nonnilable */

  writeln("3a ", nilableLHS);
  nilableLHS = new owned object();
  writeln("3b ", nilableLHS);
  nilableLHS = new owned object();
  writeln("3c ", nilableLHS);

  /* nonnilable <- nilable reports compiler error - tested elsewhere */
  // nonnlbLHS = nilableLHS;

  /* nonnilable <- nonnilable when RHS is nil */
  // When all holes are plugged, this section is no longer relevant.

  writeln("4a ", nonnlbLHS);
  nonnlbLHSb = NILA[1].borrow(); // should generate runtime error
  writeln("4b ", nonnlbLHSb);
}
