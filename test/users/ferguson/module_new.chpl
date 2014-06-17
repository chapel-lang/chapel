module M {
  class X {
    var x:int;
  }
}

module Y {
  proc main() {
    var y = new M.X(1);
  }
}
