class C1 {
  var x: int;
  proc deinit () {writeln("Inside ~C1");}
}

class C2 {
  var y: int;
  proc deinit () {writeln("Inside ~C2");}
}

class C: C1, C2 {
  proc deinit () {writeln("Inside ~C");}
}

var c = new C();
delete c;
