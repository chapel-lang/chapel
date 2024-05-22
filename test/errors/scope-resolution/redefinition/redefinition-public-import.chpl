module Lib1 {
  var a: int;
}

module Lib2 {
  var a: real;
}

module T1 {
  public import Lib1.a, Lib2.a;
  proc main() { }
}
