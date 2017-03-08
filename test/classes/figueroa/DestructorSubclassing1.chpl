class C1 {
  var x: int;
  proc deinit () {writeln("Inside ~C1");}
}

class C2: C1 {
  var y: int;
  proc deinit () {writeln("Inside ~C2");}
}

var c2: C2 = new C2();
delete c2;
