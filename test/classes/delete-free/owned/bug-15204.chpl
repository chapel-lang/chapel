class C {
  proc init() {
    writeln("in class C init()");
  }
  proc init(arg: owned C) {
    writeln("in class C init(owned C)");
  }
}

proc foo(type t) {
  var own = new owned C();
  var x = new t(own);
}

proc main() {
  foo(owned C);
}
