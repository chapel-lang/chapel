
class C {
  var x: int;
  proc deinit { writeln("C.deinit"); }
}

var c = new C();
delete c;
