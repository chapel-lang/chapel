class D {
  var me: int;
}

class C {
  var Ds: [LocaleSpace] unmanaged D?;

  proc writeThis(x) {
    for loc in Locales do
      on loc do
	Ds[loc.id]!.writeThis(x);
  }
}

var myC = new unmanaged C();

for loc in Locales do
  on loc do
    myC.Ds[loc.id] = new unmanaged D(me=loc.id);

writeln(myC);

for loc in Locales do
  on loc do
    delete myC.Ds[loc.id];

delete myC;


