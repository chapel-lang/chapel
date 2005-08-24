class C : value {
  var x : integer = 1;
  var y : float = 2.0;
}

class D : C {
  var z : string = "three";
}

var c : C, d : D;

writeln(c);
writeln(d);
