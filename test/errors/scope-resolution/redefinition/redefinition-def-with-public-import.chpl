module Lib1 {
  var a: int;
}
module T2 {
  public import Lib1.a;
  var a: real;
  proc main() { }
}
