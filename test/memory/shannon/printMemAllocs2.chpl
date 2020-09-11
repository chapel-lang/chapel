use Memory;

class C {
  var a: [1..320] int(64);
}

var c = new unmanaged C();
var d = new unmanaged C();
var e = new unmanaged C();
var f = new unmanaged C();

printMemAllocs(2400);

delete c;
delete d;
delete e;
delete f;
