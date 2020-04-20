
class LocC {
  type idxType;

  var x: idxType;
}


class C {
  type idxType = int(64);

  const targetLocs = Locales;

  var D: domain(locale) = targetLocs;

  var A: [D] unmanaged LocC(idxType)?;

  proc postinit() {
    for (loc, locid) in zip(targetLocs, 0..) do
      on loc do
        A(loc) = new unmanaged LocC(idxType);
  }
}

var myC = new unmanaged C(targetLocs=Locales);

writeln("myC = ", myC);

for loc in myC.targetLocs do
  on loc do
    delete myC.A(loc);

delete myC;
