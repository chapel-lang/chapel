_extern def printMemTable(thresh=1000, lineno=-1, filename="");

class C {
  var a: [1..32] int(64);
}

var c = new C();
var d = new C();
var e = new C();
var f = new C();

printMemTable(240, 0, "");
