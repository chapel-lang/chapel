
proc useit(ARG1, ARG2) return ARG2;

const loc = Locales[0]; // intentionally the same names as ind vars below
const loc1 = loc;
const loc2 = loc;

proc main {
  forall loc in Locales with (
    var tpv1= useit(loc, numLocales),
    var tpv2: loc.type
  ) {
    writeln(loc, tpv1,tpv2);
  }

  forall (loc1,loc2) in zip(Locales,Locales) with (
    var tpv1= useit(loc1, numLocales),
    var tpv2: loc2.type
  ) {
    writeln(loc1, loc2, tpv1, tpv2);
  }
}
