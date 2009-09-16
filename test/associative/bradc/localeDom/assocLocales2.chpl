
class C {
  type idxType = int(64);

  const targetLocs = Locales;

  var D: domain(locale) = targetLocs;
}

var myC = new C(targetLocs=Locales);

writeln("myC = ", myC);

delete myC;
