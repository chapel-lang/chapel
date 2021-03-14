class C1 {
  var x: int;
  proc deinit () {writeln("Inside ~C1");}
}

class C2: C1 {
  var y: int;
  proc deinit () {writeln("Inside ~C2");}
}

var c2: borrowed C2 = new borrowed C2();
