module Lib1 {
  var a: int;
}

module Lib2 {
  var a: real;
}

module T1 {
  public use Lib1, Lib2;
  proc main() { }
}
