
class C {
  var x: int;
  proc deinit { writeln("C.deinit"); }
}

var c = new unmanaged C();
delete c;
