module A {
  var x: int;
}
module B {
  private import A.x;
}
module C {
  import B.{x};
}
