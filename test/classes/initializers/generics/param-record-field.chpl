record R { }

class C {
  param r:R;
  proc init() { }
}

var c = new borrowed C();
writeln(c);
