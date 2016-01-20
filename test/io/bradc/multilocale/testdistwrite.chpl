class D {
  var me: int;
}

class C {
  var Ds: [LocaleSpace] D;

  proc writeThis(x) {
    for loc in Locales do
      on loc do
	Ds[loc.id].writeThis(x);
  }
}

var myC = new C();

for loc in Locales do
  on loc do
    myC.Ds[loc.id] = new D(me=loc.id);

writeln(myC);
