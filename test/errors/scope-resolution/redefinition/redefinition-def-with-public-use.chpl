module Lib1 {
  var a: int;
}
module T2 {
  public use Lib1;
  var a: real;
  proc main() { }
}
