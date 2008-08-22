
class LocC {
  type idxType;

  var x: idxType;
}


class C {
  type idxType = int(64);

  const targetLocs = Locales;

  var D: domain(locale) = targetLocs;

  var A: [D] LocC(idxType);

  def initialize() {
    for (loc, locid) in (targetLocs, 0..) do
      on loc do
        A(loc) = new LocC(idxType);
  }
}

var myC = new C(targetLocs=Locales);

writeln("myC = ", myC);
