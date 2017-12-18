
var kkkk = 8000;
var llll = 7000;
var mmmm = 6000;

proc main {
  forall jjjj in MOO() with (
    in kkkk,
    var xxxx = kkkk + 100,
    var yyyy = xxxx + llll
  ) {
    writeln("===");
    writeln(jjjj);
    writeln(kkkk);
    writeln(mmmm);
    writeln(xxxx);
    writeln(yyyy);
    kkkk += 11;
    xxxx += 22;
    yyyy += 33;
    writeln();
  }
}

iter MOO() { yield 5; }

// Ensure serial/deterministic execution.
iter MOO(param tag) where tag == iterKind.standalone {
  var s$: sync int = 0;
  coforall 1..2 {
    const uuuu = s$ + 1;
    writeln("start ", uuuu);
    for vvvv in 1..3 do
      yield uuuu * 1000 + vvvv;
    writeln("done  ", uuuu);
    s$ = uuuu;
  }
}
