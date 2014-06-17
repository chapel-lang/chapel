class Outer {
  var s = "Outer";
  class Inner {
    var m: int;
    proc get_s() {
      return get_my_s();
    }
  }
  proc makeAnInner(i: int) {
    return new Inner(i);
  }
  proc get_my_s() { return s; }
}

class ExtendOuter: Outer {
  var s = "ExtendOuter";
  proc get_my_s() { return s; }
}

proc main {
  var outer = new Outer();
  var inner = outer.makeAnInner(1);
  inner.outer = new ExtendOuter();
  writeln(inner.get_s());

}
