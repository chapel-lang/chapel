class Outer {
  var s = "Outer";
  class Inner {
    var m: int;
    proc get_s() {
      return get_my_s();
    }
  }
  proc makeAnInner(i: int) {
    return new unmanaged Inner(i);
  }
  proc get_my_s() { return s; }
}

class ExtendOuter: Outer {
  var s = "ExtendOuter";
  override proc get_my_s() { return s; }
}

proc main {
  var outer = new unmanaged Outer();
  var inner = outer.makeAnInner(1);

  var new_outer = new unmanaged ExtendOuter();
  inner.outer = new_outer;
  writeln(inner.get_s());

  delete new_outer;
  delete inner;
  delete outer;
}
