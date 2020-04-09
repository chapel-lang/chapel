/* This test focuses on boundary cases, e.g.
   no elements, a single element, last row, out-of-bounds, etc.
*/

use LayoutCS;

const rr = 4, cc = 5;

var dd = {1..rr, 1..cc};
var sd: sparse subdomain(dd) dmapped new dmap(new CS());

var cnt = 0;
proc show(msg...) {
  cnt += 1;
  writeln(cnt, ": ", (...msg));
  forall i in sd do writeln(cnt, ": ", i);
}

proc test(msg) {
  sd += (3,4);
  show(msg, "added (3,4)");

  sd -= (3,4);
  show(msg, "removed (3,4)");

  sd += (1,3);
  show(msg, "added (1,3)");

  sd -= (1,3);
  show(msg, "removed (1,3)");

  sd += (4,3);
  show(msg, "added (4,3)");

  sd -= (4,3);
  show(msg, "removed (4,3)");
}

show("empty");
test("");

sd += (1,1); sd += (rr,cc);
test("with (1,1) and (rr,cc) - ");

sd -= (1,1); sd -= (rr,cc);
show("empty");

// this should give an out-of-bounds error
sd += (rr+1,1);
