class C1 {
  var x: int;
  def ~C1 () {writeln("Inside ~C1");}
}

class C2 {
  def ~C2 () {writeln("Inside ~C2");}
}

class C: C1, C2 {
  def ~C () {writeln("Inside ~C");}
}

class D {
  var y: int;
  def ~D () {writeln("Inside ~D");}
}

class E: C, D {
  def ~E () {writeln("Inside ~E");}
}

var e: new E();
delete e;
