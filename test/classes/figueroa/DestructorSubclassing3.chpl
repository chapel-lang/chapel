class C1 {
  var x: int;
  proc deinit () {writeln("Inside ~C1");}
}

class C2 {
  proc deinit () {writeln("Inside ~C2");}
}

class C: C1, C2 {
  proc deinit () {writeln("Inside ~C");}
}

class D {
  var y: int;
  proc deinit () {writeln("Inside ~D");}
}

class E: C, D {
  proc deinit () {writeln("Inside ~E");}
}

var e = new E();
delete e;
