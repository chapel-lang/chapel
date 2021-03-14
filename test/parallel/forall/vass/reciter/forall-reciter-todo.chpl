/*
See ./forall-reciter-works.chpl

This test is for reduce intents, which are TODO.
*/

config const nn = 3;
config const mm = 3;

//
// myiter() iterator ensures deterministic execution (using cnt$)
// while still creating bona fide parallel tasks
//

iter myiter(depth: int): int {
  yield 555;
}

iter myiter(depth: int, param tag: iterKind): int
   where tag == iterKind.standalone
{
  if depth <= 0 {
    //
    // this piece ensures deterministic execution (using cnt$)
    // while still creating bona fide parallel tasks
    //
    var cnt$: sync int = 1;
    coforall ooo in 1..nn {
      const current = cnt$.readFE();
      writef("myiter start %t\n", current);
      for jjj in 1..mm {
        yield current * 100 + jjj;
      }
      writef("myiter done  %t\n", current);
      cnt$.writeEF(current + 1);
    }
  } else {
    writeln("recursing for depth ", depth);
    // the recursive piece
    forall kkk in myiter(depth-1) do
      yield kkk;
  }
}

proc main {
  writeln("starting");

  // testing reduce intents
  var ovarPR = 1;
  forall iii2 in myiter(2) with (+ reduce ovarPR) {
    writef("%5i  %5i\n", iii2, ovarPR);
    ovarPR += iii2;
  }
  writef("done iii2  %i\n", ovarPR);

  // testing in intents
  var ovarIn = 1;
  forall iii3 in myiter(2) with (in ovarIn) {
    writef("%5i  %5i\n", iii3, ovarIn);
    ovarIn += iii3;
  }
  writef("done iii3  %i\n", ovarIn);
}
