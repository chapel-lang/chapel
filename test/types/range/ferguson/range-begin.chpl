/* This test highlights problems I was seeing with
     studies/cholesky/jglewis/test_dataflow_cholesky.chpl
     studies/cholesky/jglewis/version2/dataflow/test_dataflow_cholesky.chpl
   on the arrays branch, after these tests were adapted to use
   ranges.
 */

use Time;

var start1$:sync int;
var end1$:sync int;
var start2$:sync int;
var end2$:sync int;

proc inner1(range_arg) {
  start1$.readFE(); // wait for run to finish
  writeln("inner1: ", range_arg);
  end1$.writeEF(1); // signal done
}

proc outer1(range_arg) {
  begin with (in range_arg) {
    inner1(range_arg);
  }
}

proc inner2(range_arg) {
  start2$.readFE(); // wait for run to finish
  writeln("inner2: ", range_arg);
  end2$.writeEF(2); // signal done
}

proc outer2(range_arg) {
  begin {
    inner2(range_arg);
  }
}


proc run() {
  var r = 1..100;
  var s$:sync int;
  begin with (ref r) {
    r = 2..200;
    s$.writeEF(0);
  }
  s$.readFE(); // wait for begin
  writeln(r);
  outer1(r);
  outer2(r);
  r = 0..0;
  // r now destroyed, don't wait
}

proc test() {
  run();
  // now that r is destroyed, let the tasks start
  start1$.writeEF(1); // let inner1 continue
  end1$.readFE(); // wait for inner1 to finish
  start2$.writeEF(2); // let inner2 continue
  end2$.readFE(); // wait for inner2 to finish
}

test();
