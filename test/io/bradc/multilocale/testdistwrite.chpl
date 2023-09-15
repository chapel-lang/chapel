class D {
  var me: int;
}

class C : writeSerializable {
  var Ds: [LocaleSpace] unmanaged D?;

  override proc serialize(writer, ref serializer) throws {
    for loc in Locales do
      on loc do
	Ds[loc.id]!.serialize(writer, serializer);
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


