use Memory;

class C {
  var a: [1..32] int(64);
}

var c = new C();
var d = new C();
var e = new C();
var f = new C();

printMemAllocs(240, true);
