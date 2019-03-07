class C {
  type eltType = real;
  var head, tail: atomic int;

  proc init() {
    head.write(0);
    tail.write(0);
  }
}

var myC = new borrowed C();
writeln(myC);
