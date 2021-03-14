
record R {
  var x : int;
}

class C {
  var t = new R(5);
  proc init() {}
}

var c = new unmanaged C();
writeln("c = ", c);
delete c;
