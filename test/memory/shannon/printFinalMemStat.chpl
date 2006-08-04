config var print: bool = false;

class C {
  var a: int(64);
  var b: int(64);
  var c: int(64);
  var d: int(64);
  var e: int(64);
  var f: int(64);
  var g: int(64);
}

var c = C();
var d = C();
var e = C();
var f = C();

if print {
  writeln(c);
  writeln(d);
  writeln(e);
  writeln(f);
}


