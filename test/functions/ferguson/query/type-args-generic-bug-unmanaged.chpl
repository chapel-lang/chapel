class C {
  var x : int;
  proc init() { }
}

class G {
  type t;
  var x : t;
  proc init(type t) { this.t = t; }
}

proc foo(type t : unmanaged G) {
  writeln("G");
}

proc foo(type t : unmanaged object) {
  writeln("object");
}

foo(unmanaged C); // prints "G" !?
foo(unmanaged G(int)); // G
