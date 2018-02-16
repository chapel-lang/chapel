record R { }

class C {
  param r:R;
  proc init() { }
}

var c = new C();
writeln(c);
delete c;

