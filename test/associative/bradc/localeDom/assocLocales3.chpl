
class LocC {
  type idxType;

  var x: idxType;
}


class C {
  type idxType = int(64);

  const targetLocs = rootLocale.getLocales();

  var D: domain(locale) = targetLocs;

  var A: [D] LocC(idxType);

  proc initialize() {
    for (loc, locid) in (targetLocs, 0..) do
      on loc do
        A(loc) = new LocC(idxType);
  }
}

var myC = new C(targetLocs=rootLocale.getLocales());

writeln("myC = ", myC);

for loc in myC.targetLocs do
  on loc do
    delete myC.A(loc);

delete myC;
