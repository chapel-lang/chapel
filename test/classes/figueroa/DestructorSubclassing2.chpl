class C1 {
  var x: int;
  def ~C1 () {writeln("Inside ~C1");}
}

class C2 {
  var y: int;
  def ~C2 () {writeln("Inside ~C2");}
}

class C: C1, C2 {
  def ~C () {writeln("Inside ~C");}
}

var c: new C();
delete c;
