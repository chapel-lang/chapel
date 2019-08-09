class C {
  var x : int;
  proc init() { }
}

class G {
  type t;
  var x : t;
  proc init(type t) { this.t = t; }
}

proc foo(type t : borrowed G) {
  writeln("G");
}

proc foo(type t : borrowed object) {
  writeln("object");
}

foo(borrowed C); // prints "G" !?
foo(borrowed G(int)); // G
