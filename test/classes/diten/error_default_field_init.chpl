
var n: real = 7.5;

class C {
  var y: int;
  var z: real;
  var xx: int = 2.0; // Error, can't implicitly convert 2.0 to int
}

var c = new borrowed C();
writeln(c);
