class D {
  type elemType;
  var x: elemType;
}

class C {
  type elemType;
  const dArray: [LocaleSpace] D(elemType);

  proc init(type elemType, targetLocales: [?targetLocalesDomain] locale) {
    this.elemType = elemType;
    this.complete();
    for locid in LocaleSpace do
      on Locales(locid) do
        dArray(locid) = new D(elemType);
  }

  proc deinit() {
    for locid in LocaleSpace do
      on Locales(locid) do
        delete dArray(locid);
  }
}

var myC = new C(real(64), Locales);
writeln("myC = ", myC);
delete myC;
