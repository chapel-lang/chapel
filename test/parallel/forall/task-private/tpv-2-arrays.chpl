
var globalArr = [4, 5, 6];

proc returnGlobalArr() {
  return globalArr;
}

proc returnLocalArr() {
  var localArr = [1, 3, 8];
  return localArr;
}

forall jjj in MOO() with (
  var tpv1 = globalArr,
  var tpv2 = returnGlobalArr(),
  var tpv3 = returnLocalArr(),
  var tpv4 = [2, 7, 9, 10]
) {
  writeln("=== ", jjj);
  writeln("tpv1 ", tpv1);
  writeln("tpv2 ", tpv2);
  writeln("tpv3 ", tpv3);
  writeln("tpv4 ", tpv4);

  tpv1[1] += 11;;
  tpv2[2] += 22;
  tpv3[3] += 30;
  tpv4[4] += 400;
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
