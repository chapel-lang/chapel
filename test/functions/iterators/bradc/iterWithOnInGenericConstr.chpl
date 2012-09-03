class D {
  type elemType;
  var x: elemType;
}

class C {
  type elemType;
  const dArray: [rootLocale.getLocaleSpace()] D(elemType);

  proc C(type elemType, targetLocales: [?targetLocalesDomain] locale) {
    for locid in rootLocale.getLocaleSpace() do
      on rootLocale.getLocales()(locid) do
        dArray(locid) = new D(elemType);
  }
}

var myC = new C(real(64), rootLocale.getLocales());
writeln("myC = ", myC);
delete myC;
