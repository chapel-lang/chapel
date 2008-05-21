class Outer {
  var s = "Outer";
  class Inner {
    var m: int;
    def get_s() {
      return get_my_s();
    }
  }
  def makeAnInner(i: int) {
    return new Inner(i);
  }
  def get_my_s() { return s; }
}

class ExtendOuter: Outer {
  var s = "ExtendOuter";
  def get_my_s() { return s; }
}

def main {
  var outer = new Outer();
  var inner = outer.makeAnInner(1);
  inner.outer = new ExtendOuter();
  writeln(inner.get_s());

}
