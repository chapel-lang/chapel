module A {
  var x: int;
}
module B {
  private use A;
}
module C {
  use B only x;
}
