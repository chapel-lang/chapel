use Memory;

class C {
  var a: [1..32] int(64);
}

var c = new unmanaged C();
var d = new unmanaged C();
var e = new unmanaged C();
var f = new unmanaged C();

printMemAllocs(240);

delete c;
delete d;
delete e;
delete f;
