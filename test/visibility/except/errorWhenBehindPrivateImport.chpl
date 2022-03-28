module A {
  var x: int;
}
module B {
  private import A.x;
}
module C {
  use B except x;
}
