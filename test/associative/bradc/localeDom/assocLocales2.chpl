
class C {
  type idxType = int(64);

  const targetLocs = rootLocale.getLocales();

  var D: domain(locale) = targetLocs;
}

var myC = new C(targetLocs=rootLocale.getLocales());

writeln("myC = ", myC);

delete myC;
