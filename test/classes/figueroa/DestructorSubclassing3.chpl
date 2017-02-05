class C1 {
  var x: int;
  proc ~C1 () {writeln("Inside ~C1");}
}

class C2 {
  proc ~C2 () {writeln("Inside ~C2");}
}

class C: C1, C2 {
  proc ~C () {writeln("Inside ~C");}
}

class D {
  var y: int;
  proc ~D () {writeln("Inside ~D");}
}

class E: C, D {
  proc ~E () {writeln("Inside ~E");}
}

var e = new E();
delete e;
