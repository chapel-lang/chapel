/*
A forall loop that invokes a recursive parallel iterator.

As of this writing (#7738), this works except for the case
where there are reduce intents.
Once those are implemented, extend this test to exercise
those as well, by incorporating ./forall-reciter-works.chpl

This situation is inspired by this test:
  modules/standard/FileSystem/filerator/bradc/findfiles-par.chpl
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
      const current = cnt$;
      writef("myiter start %t\n", current);
      for jjj in 1..mm {
        yield current * 100 + jjj;
      }
      writef("myiter done  %t\n", current);
      cnt$ = current + 1;
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

  // no shadow variables at all
  forall iii0 in myiter(2) {
    writeln(iii0);
  }
  writeln("done iii0");

  // testing in and ref intents
  var ovarIn = 1, ovarRef = 2;
  forall iii2 in myiter(2) with (in ovarIn, ref ovarRef) {
    writef("%5i  %5i  %5i\n", iii2, ovarIn, ovarRef);
    ovarIn += iii2;
    ovarRef += iii2;
  }
  writef("done iii2  %i  %i\n", ovarIn, ovarRef);
}
