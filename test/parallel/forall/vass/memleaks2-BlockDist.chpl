use Memory;
use BlockDist;

const SPACE = {1..2, 1..2};
const DDOM = SPACE dmapped Block(SPACE);
var DARRAY: [DDOM] int;

config const initval = 1;

proc main() {
  var myvar = initval;
  const m1 = memoryUsed();

  // serial to quiet a sporadic (1 in 1000) regression (see JIRA issue 112)
  serial do
    forall da in DARRAY do
      da = myvar;

  const m2 = memoryUsed();

  writeln(DARRAY);
  writeln("leaked: ", m2-m1);
}
