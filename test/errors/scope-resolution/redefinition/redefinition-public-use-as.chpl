module Lib1 {
  var a: int;
}

module Lib2 {
  var b: real;
}

module T1 {
  public use Lib1;
  public use Lib2 only b as a;
  proc main() { }
}
