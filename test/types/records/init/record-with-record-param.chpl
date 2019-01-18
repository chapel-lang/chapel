record R { }

record RR {
  param r:R;
  proc init() {}
}

var rr = new RR();
writeln(rr);
