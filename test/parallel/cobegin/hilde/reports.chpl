// reports.chpl
//
// Test per-locale task table operation.
//

use Time;

var a$ : sync bool;

forall i in 0..#numLocales do
  on Locales[i] do {
    cobegin {
      {sleep(1); if a$ then writeln("impossible"); }
      {sleep(1); if a$ then writeln("impossible"); }
      {sleep(1); if a$ then writeln("impossible"); }
      {sleep(1); if a$ then writeln("impossible"); }
    }
    if a$ then writeln("impossible");
  }
