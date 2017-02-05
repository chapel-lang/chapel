class C1 {
  var x: int;
  proc ~C1 () {writeln("Inside ~C1");}
}

class C2 {
  var y: int;
  proc ~C2 () {writeln("Inside ~C2");}
}

class C: C1, C2 {
  proc ~C () {writeln("Inside ~C");}
}

var c = new C();
delete c;
