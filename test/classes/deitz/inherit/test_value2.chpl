record C {
  var x : int = 1;
  var y : real = 2.0;
}

record D : C {
  var z : string = "three";
}

var c : C, d : D;

writeln(c);
writeln(d);
