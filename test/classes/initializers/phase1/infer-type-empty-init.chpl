
record R {
  var x : int;
}

class C {
  var t = new R(5);
  proc init() {}
}

var c = new C();
writeln("c = ", c);
delete c;
